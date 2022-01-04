#include "AST/AST.h"

using namespace AST;

unsigned Block::total_blocks = 0;

static void rewrite_branches(std::vector<Block>& bloks, unsigned cur_bb, unsigned next_bb) {
    auto& bb = bloks[cur_bb];

    auto last_inst = bb.lines.back();
    if(last_inst->is_brnch()) {
        auto casted_inst = reinterpret_cast<CondBranchInst*>(last_inst);
        rewrite_branches(bloks, casted_inst->bb_next_, next_bb);
        rewrite_branches(bloks, casted_inst->bb_cond_, casted_inst->bb_next_);
    } else {
        // need insert
        std::cout << "Insert last bb" << std::endl;
        bb.lines.push_back(new BranchInst(next_bb));
    }
}

void AST::complete_cfg(ASTContext& ctx) {
    auto& bloks = ctx.blocks;
    auto entry_bb_id = bloks.size() - 1; 
    auto special_bb_id = bloks.size(); /* This bb will contain only ret operation */
    rewrite_branches(bloks, entry_bb_id, special_bb_id);
}