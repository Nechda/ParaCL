#include "Codegen/CGVisitor.h"
#include <cassert>

void CodegenerationVisitor::clear_value_stack() { value_stack_ = {}; }

/* This method keep valid list of variables */
void CodegenerationVisitor::height_align(int height) {
    assert(height >= 0);

    while (blocks_variable.size() > height) {
        blocks_variable.pop_back();
    }

    blocks_variable.push_back({});
}

llvm::Value *CodegenerationVisitor::search_variable(std::string &name) {
    llvm::Value *result = nullptr;

    auto it = blocks_variable.rbegin();
    auto end_it = blocks_variable.rend();
    for (; it != end_it && !result; ++it) {
        auto &mapa = *it;
        auto search_it = mapa.find(name);
        result = search_it != mapa.end() ? search_it->second : result;
    }

    return result;
}

void CodegenerationVisitor::visit(AST::ConstLineral &node) { value_stack_.push(ir_builder.getInt32(node.val_)); }

void CodegenerationVisitor::visit(AST::BinaryInst &node) {
    node.lhs_->on_visit(this);
    auto lhs_value = value_stack_.top();
    value_stack_.pop();

    node.rhs_->on_visit(this);
    auto rhs_value = value_stack_.top();
    value_stack_.pop();

    /* Load value of variables */
    lhs_value = node.lhs_->is_ptr() ? ir_builder.CreateLoad(lhs_value) : lhs_value;
    rhs_value = node.rhs_->is_ptr() ? ir_builder.CreateLoad(rhs_value) : rhs_value;

    switch (node.op_) {
    case AST::BinaryInst::OP::ADD:
        value_stack_.push(ir_builder.CreateAdd(lhs_value, rhs_value));
        return;
    case AST::BinaryInst::OP::SUB:
        value_stack_.push(ir_builder.CreateSub(lhs_value, rhs_value));
        return;
    case AST::BinaryInst::OP::MUL:
        value_stack_.push(ir_builder.CreateMul(lhs_value, rhs_value));
        return;
    case AST::BinaryInst::OP::DIV:
        value_stack_.push(ir_builder.CreateFDiv(lhs_value, rhs_value));
        return;

    case AST::BinaryInst::OP::GT:
        value_stack_.push(ir_builder.CreateICmpSGT(lhs_value, rhs_value));
        break;
    case AST::BinaryInst::OP::LT:
        value_stack_.push(ir_builder.CreateICmpSLT(lhs_value, rhs_value));
        break;
    case AST::BinaryInst::OP::GE:
        value_stack_.push(ir_builder.CreateICmpSGE(lhs_value, rhs_value));
        break;
    case AST::BinaryInst::OP::LE:
        value_stack_.push(ir_builder.CreateICmpSLE(lhs_value, rhs_value));
        break;

    case AST::BinaryInst::OP::EQ:
        value_stack_.push(ir_builder.CreateICmpEQ(lhs_value, rhs_value));
        break;
    case AST::BinaryInst::OP::NE:
        value_stack_.push(ir_builder.CreateICmpNE(lhs_value, rhs_value));
        break;
    default:
        break;
    }

    // need cast to i32
    auto i1_value = value_stack_.top();
    value_stack_.pop();
    value_stack_.push(ir_builder.CreateIntCast(i1_value, ir_builder.getInt32Ty(), 1));
}

void CodegenerationVisitor::visit(AST::CondBranchInst &node) {
    auto bb_cond = bb_map[node.bb_cond_];
    auto bb_next = bb_map[node.bb_next_];

    node.cond_->on_visit(this);
    auto cond_value = value_stack_.top();
    value_stack_.pop();

    cond_value = node.cond_->is_ptr() ? ir_builder.CreateLoad(cond_value) : cond_value;
    cond_value = ir_builder.CreateICmpNE(cond_value, ir_builder.getInt32(0)); /* Hack for i32 -> i1 */
    value_stack_.push(ir_builder.CreateCondBr(cond_value, bb_cond, bb_next));
}

void CodegenerationVisitor::visit(AST::BranchInst &node) {
    auto bb_next = bb_map[node.bb_next_];
    value_stack_.push(ir_builder.CreateBr(bb_next));
}

void CodegenerationVisitor::visit(AST::Variable &node) { value_stack_.push(search_variable(node.name_)); }

void CodegenerationVisitor::visit(AST::AssignInst &node) {
    node.rhs_->on_visit(this);
    auto rhs_value = value_stack_.top();
    value_stack_.pop();

    /* Load value of variable */
    rhs_value = node.rhs_->is_ptr() ? ir_builder.CreateLoad(rhs_value) : rhs_value;

    node.lhs_->on_visit(this);
    auto lhs_addr = value_stack_.top();
    value_stack_.pop();
    if (lhs_addr == nullptr) {
        // dont panic, we just create new variable
        auto var_node = *reinterpret_cast<AST::Variable *>(node.lhs_);
        auto &last_mapa = blocks_variable.back();

        auto var_ptr = ir_builder.CreateAlloca(ir_builder.getInt32Ty(), 0, var_node.name_ + "_ptr");
        last_mapa[var_node.name_] = var_ptr;
        lhs_addr = var_ptr;
    }

    ir_builder.CreateStore(rhs_value, lhs_addr);
    value_stack_.push(rhs_value);
}

void CodegenerationVisitor::visit(AST::FunctionCall &node) {
    node.arg_->on_visit(this);
    auto arg_value = value_stack_.top();
    value_stack_.pop();

    arg_value = node.arg_->is_ptr() ? ir_builder.CreateLoad(arg_value) : arg_value;

    value_stack_.push(ir_builder.CreateCall(func_map[node.name_], {arg_value}));
}
