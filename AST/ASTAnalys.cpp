#include "AST/AST.h"
#include "ScopePtr.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <limits>
#include <vector>

#define DUMP_GRAPH 0

using namespace AST;

unsigned Block::total_blocks = 0;

static std::vector<ExprBase *> branch_inst_stack;

AST::ExprBase *insert_helper(std::vector<AST::ExprBase *> &vec, AST::ExprBase *elem);

void AST::ASTContext::rewrite_branches(unsigned cur_bb) {
    auto &bb = blocks[cur_bb];

    if (bb.reason == Block::BranchReason::NONE) {
        bb.lines.push_back(branch_inst_stack.back());
        branch_inst_stack.pop_back();
        return;
    }

    if (bb.reason == Block::BranchReason::IF) {
        auto br_inst = dynamic_cast<CondBranchInst *>(bb.lines.back());

        insert_helper(nodes, new BranchInst(br_inst->bb_next_));
        branch_inst_stack.push_back(nodes.back());
        rewrite_branches(br_inst->bb_cond_);

        rewrite_branches(br_inst->bb_next_);
        return;
    }

    if (bb.reason == Block::BranchReason::WHILE) {
        auto br_inst = dynamic_cast<CondBranchInst *>(bb.lines.back());

        branch_inst_stack.push_back(bb.lines.back());
        rewrite_branches(br_inst->bb_cond_);

        rewrite_branches(br_inst->bb_next_);
        return;
    }
}

void AST::ASTContext::complete_cfg() {
    auto &bloks = blocks;
    auto entry_bb_id = bloks.size() - 1;
    auto special_bb_id = bloks.size(); /* This bb will contain only ret operation */

    insert_helper(nodes, new BranchInst(special_bb_id));
    branch_inst_stack.push_back(nodes.back());
    rewrite_branches(entry_bb_id);
}

using namespace boost;
using G = adjacency_list<listS, listS, bidirectionalS, property<vertex_index_t, std::size_t>, no_property>;
using Edge_t = std::pair<int, int>;

static std::vector<Edge_t> build_dominator_tree(std::vector<Edge_t> &edges, const int n_vertices) {
    G graph(edges.begin(), edges.end(), n_vertices);
    const auto null_vert = graph_traits<G>::null_vertex();
    using Vertex = graph_traits<G>::vertex_descriptor;
    using VertexIt = graph_traits<G>::vertex_iterator;
    using IndexMap = property_map<G, vertex_index_t>::type;
    using PredMap = iterator_property_map<std::vector<Vertex>::iterator, IndexMap>;

    IndexMap indexMap(get(vertex_index, graph));

    VertexIt uItr, uEnd;
    int idx = 0;

    for (tie(uItr, uEnd) = vertices(graph); uItr != uEnd; ++uItr, ++idx) {
        put(indexMap, *uItr, idx);
    }
#if DUMP_GRAPH
    write_graphviz(std::cout, graph);
#endif

    // Lengauer-Tarjan dominator tree algorithm
    std::vector<Vertex> domTreePredVector(n_vertices, null_vert);
    PredMap domTreePredMap = make_iterator_property_map(domTreePredVector.begin(), indexMap);

    lengauer_tarjan_dominator_tree(graph, vertex(0, graph), domTreePredMap);
    std::vector<int> idom(n_vertices);
    for (tie(uItr, uEnd) = vertices(graph); uItr != uEnd; ++uItr) {
        auto &idom_cell = idom[get(indexMap, *uItr)];
        auto pred = get(domTreePredMap, *uItr);
        idom_cell = pred != null_vert ? get(indexMap, pred) : std::numeric_limits<int>::max();
    }

    std::vector<Edge_t> result;
    result.push_back({0, 0});
    for (int k = 1; k < idom.size(); k++) {
        result.push_back(Edge_t(k, idom[k]));
    }

#if DUMP_GRAPH
    G g2(result.begin(), result.end(), n_vertices);
    idx = 0;
    for (tie(uItr, uEnd) = vertices(g2); uItr != uEnd; ++uItr, ++idx)
        put(indexMap, *uItr, idx);

    write_graphviz(std::cout, g2);
    std::cout << std::endl;
#endif

    return result;
}

static std::vector<Edge_t> get_ast_edges(ASTContext &ctx, bool ignore_special = true) {
    std::vector<Edge_t> edges;
    for (auto &bb : ctx.blocks) {
        for (auto &line : bb.lines) {
            if (!line->is_brnch())
                continue;

            auto branch_base = dynamic_cast<BranchInstBase *>(line);
            if (branch_base->get_type() == BranchInstBase::BranchType::Uncondition) {
                auto br_inst = dynamic_cast<BranchInst *>(branch_base);
                edges.push_back({bb.ID, br_inst->bb_next_});
            } else {
                auto br_inst = dynamic_cast<CondBranchInst *>(branch_base);
                edges.push_back({bb.ID, br_inst->bb_next_});
                edges.push_back({bb.ID, br_inst->bb_cond_});
            }
        }
    }

    /* Remove edges to special blocks */
    edges.erase(std::remove_if(edges.begin(), edges.end(),
                               [&](const Edge_t &edge) {
                                   const auto special_id = ctx.blocks.size();
                                   return ignore_special && (edge.first == special_id || edge.second == special_id);
                               }),
                edges.end());

    return edges;
}

void AST::ASTContext::generate_dominator_tree() {
    auto inv = [&](Edge_t &cur) {
        cur.first = blocks.size() - 1 - cur.first;
        cur.second = blocks.size() - 1 - cur.second;
    };

    auto ast_edges = get_ast_edges(*this);

    // Need inverse indices of blocks
    std::for_each(ast_edges.begin(), ast_edges.end(), inv);

    auto dom_edges_local = build_dominator_tree(ast_edges, blocks.size());

    // Inverse for getting actual indices
    std::for_each(dom_edges_local.begin(), dom_edges_local.end(), inv);

    std::sort(dom_edges_local.begin(), dom_edges_local.end(), [](Edge_t &a, Edge_t &b) { return a.first < b.first; });
    dom_edges = std::move(dom_edges_local);
}

std::vector<int> AST::ASTContext::create_dfs_order() {
    if (blocks.size() == 1) {
        return {0};
    }

    std::vector<int> tmp;
    std::vector<int> dfs_order;

    std::vector<bool> is_leaf(blocks.size(), 1);
    for (auto &p : dom_edges) {
        is_leaf[p.second] = 0;
    }

    std::vector<bool> is_marked(blocks.size(), 0);
    for (int i = 0; i < is_leaf.size(); i++) {
        if (!is_leaf[i])
            continue;

        tmp.clear();
        int cur_node = i;
        while (!is_marked[cur_node]) {
            is_marked[cur_node] = 1;
            tmp.push_back(cur_node);
            cur_node = dom_edges[cur_node].second;
        }

        while (!tmp.empty()) {
            auto last = tmp.back();
            dfs_order.push_back(last);
            tmp.pop_back();
        }
    }

    return dfs_order;
}

std::vector<int> AST::ASTContext::create_height_array() {
    if (blocks.size() == 1) {
        return {0};
    }

    std::vector<int> height_map(blocks.size(), -1);
    std::vector<int> tmp;

    std::vector<bool> is_leaf(blocks.size(), 1);
    for (auto &p : dom_edges) {
        is_leaf[p.second] = 0;
    }

    std::vector<bool> is_marked(blocks.size(), 0);
    for (int i = 0; i < is_leaf.size(); i++) {
        if (!is_leaf[i])
            continue;

        tmp.clear();
        int cur_node = i;
        while (!is_marked[cur_node]) {
            is_marked[cur_node] = 1;
            tmp.push_back(cur_node);
            cur_node = dom_edges[cur_node].second;
        }

        while (!tmp.empty()) {
            auto last = tmp.back();
            auto idom = dom_edges[last].second;

            height_map[last] = last == idom ? 0 : 1 + height_map[idom];
            tmp.pop_back();
        }
    }

    return height_map;
}
