#include "AST/AST.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <limits>
#include <vector>

#define DUMP_GRAPH 0

using namespace AST;

unsigned Block::total_blocks = 0;

using Edge_t = std::pair<int, int>;
static std::vector<Edge_t> ast_edges;

static void rewrite_branches(std::vector<Block>& bloks, unsigned cur_bb, unsigned next_bb) {
    auto& bb = bloks[cur_bb];

    auto last_inst = bb.lines.back();
    if(last_inst->is_brnch()) {
        auto casted_inst = reinterpret_cast<CondBranchInst*>(last_inst);
        if(casted_inst->bb_next_ < bloks.size()) ast_edges.push_back({cur_bb, casted_inst->bb_next_});
        if(casted_inst->bb_cond_ < bloks.size()) ast_edges.push_back({cur_bb, casted_inst->bb_cond_});
        rewrite_branches(bloks, casted_inst->bb_next_, next_bb);
        rewrite_branches(bloks, casted_inst->bb_cond_, casted_inst->bb_next_);
    } else {
        // need insert
        if(next_bb < bloks.size()) ast_edges.push_back({cur_bb, next_bb});
        bb.lines.push_back(new BranchInst(next_bb));
    }
}

void AST::complete_cfg(ASTContext& ctx) {
    auto& bloks = ctx.blocks;
    auto entry_bb_id = bloks.size() - 1; 
    auto special_bb_id = bloks.size(); /* This bb will contain only ret operation */
    rewrite_branches(bloks, entry_bb_id, special_bb_id);
}


using namespace boost;
using G = adjacency_list<listS, listS, bidirectionalS, property<vertex_index_t, std::size_t>, no_property>;

static std::vector<Edge_t> build_dominator_tree(std::vector<Edge_t>& edges, const int n_vertices) {
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
    for (tie(uItr, uEnd) = vertices(graph); uItr != uEnd; ++uItr)
    {
        auto& idom_cell = idom[get(indexMap, *uItr)];
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

void AST::generate_dominator_tree(ASTContext& ctx) {
    auto inv = [&](Edge_t& cur) {
        cur.first = ctx.blocks.size() - 1 - cur.first;
        cur.second = ctx.blocks.size() - 1 - cur.second;
    };

    // Need inverse indices of blocks
    std::for_each(ast_edges.begin(), ast_edges.end(), inv);

    auto dom_edges = build_dominator_tree(ast_edges, ctx.blocks.size());

    // Inverse for getting actual indices
    std::for_each(dom_edges.begin(), dom_edges.end(), inv);

    std::sort(dom_edges.begin(), dom_edges.end(), [](Edge_t& a, Edge_t& b) { return a.first < b.first; });
    ctx.dom_edges = std::move(dom_edges);
}

std::vector<int> AST::create_dfs_order(AST::ASTContext& ast) {
    std::vector<int> tmp;
    std::vector<int> dfs_order;

    std::vector<bool> is_leaf(ast.blocks.size(), 1);
    for(auto& p : ast.dom_edges) {
        is_leaf[p.second] = 0;
    }

    std::vector<bool> is_marked(ast.blocks.size(), 0);
    for(int i = 0; i < is_leaf.size(); i++) {
        if(!is_leaf[i]) continue;

        tmp.clear();
        int cur_node = i;
        while(!is_marked[cur_node]) {
            is_marked[cur_node] = 1;
            tmp.push_back(cur_node);
            cur_node = ast.dom_edges[cur_node].second;
        }

        while(!tmp.empty()) {
            auto last = tmp.back();
            dfs_order.push_back(last);
            tmp.pop_back();
        }
    }

    return dfs_order;
}

std::vector<int> AST::create_height_array(AST::ASTContext& ast) {
    std::vector<int> height_map(ast.blocks.size(), -1);
    std::vector<int> tmp;

    std::vector<bool> is_leaf(ast.blocks.size(), 1);
    for(auto& p : ast.dom_edges) {
        is_leaf[p.second] = 0;
    }

    std::vector<bool> is_marked(ast.blocks.size(), 0);
    for(int i = 0; i < is_leaf.size(); i++) {
        if(!is_leaf[i]) continue;

        tmp.clear();
        int cur_node = i;
        while(!is_marked[cur_node]) {
            is_marked[cur_node] = 1;
            tmp.push_back(cur_node);
            cur_node = ast.dom_edges[cur_node].second;
        }

        while(!tmp.empty()) {
            auto last = tmp.back();
            auto idom = ast.dom_edges[last].second;

            height_map[last] = last == idom ? 0 : 1 + height_map[idom];
            tmp.pop_back();
        }
    }

    return height_map;
}
