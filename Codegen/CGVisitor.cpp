#include "Codegen/CGVisitor.h"

void CodegenerationVisitor::clear_value_stack() {
    value_stack_ = {};
}

void CodegenerationVisitor::visit(AST::ConstLineral& node) {
    value_stack_.push(ir_builder.getInt32(node.val_));
}

void CodegenerationVisitor::visit(AST::BinaryInst& node) {
    node.lhs_->on_visit(*this);
    auto lhs_value = value_stack_.top();
    value_stack_.pop();

    node.rhs_->on_visit(*this);
    auto rhs_value = value_stack_.top();
    value_stack_.pop();

    /* Load value of variables */
    lhs_value = node.lhs_->is_ptr() ? ir_builder.CreateLoad(lhs_value) : lhs_value;
    rhs_value = node.rhs_->is_ptr() ? ir_builder.CreateLoad(rhs_value) : rhs_value;

    switch (node.op_) {
        case AST::BinaryInst::OP::ADD: value_stack_.push(ir_builder.CreateAdd(lhs_value, rhs_value)); break;
        case AST::BinaryInst::OP::SUB: value_stack_.push(ir_builder.CreateSub(lhs_value, rhs_value)); break;
        case AST::BinaryInst::OP::MUL: value_stack_.push(ir_builder.CreateMul(lhs_value, rhs_value)); break;
        case AST::BinaryInst::OP::DIV: value_stack_.push(ir_builder.CreateFDiv(lhs_value, rhs_value));break;
        default: break;
    }
}

void CodegenerationVisitor::visit(AST::CondBranchInst& node) {
    auto bb_cond = bb_map[node.bb_cond_];
    auto bb_next = bb_map[node.bb_next_];

    node.cond_->on_visit(*this);
    auto cond_value = value_stack_.top();
    value_stack_.pop();

    cond_value = node.cond_->is_ptr() ? ir_builder.CreateLoad(cond_value) : cond_value;
    cond_value = ir_builder.CreateICmpNE(cond_value, ir_builder.getInt32(0)); /* Hack for i32 -> i1 */
    value_stack_.push(ir_builder.CreateCondBr(cond_value, bb_cond, bb_next));
}

void CodegenerationVisitor::visit(AST::BranchInst& node) {
    auto bb_next = bb_map[node.bb_next_];
    value_stack_.push(ir_builder.CreateBr(bb_next));
}

void CodegenerationVisitor::visit(AST::Variable& node) {
    value_stack_.push(named_value[node.name_]);
}

void CodegenerationVisitor::visit(AST::AssignInst& node) {
    node.rhs_->on_visit(*this);
    auto rhs_value = value_stack_.top();
    value_stack_.pop();

    node.lhs_->on_visit(*this);
    auto lhs_addr = value_stack_.top();
    value_stack_.pop();

    ir_builder.CreateStore(rhs_value, lhs_addr);
    value_stack_.push(rhs_value);
}

void CodegenerationVisitor::visit(AST::FunctionCall& node) {
    node.arg_->on_visit(*this);
    auto arg_value = value_stack_.top();
    value_stack_.pop();

    arg_value = node.arg_->is_ptr() ? ir_builder.CreateLoad(arg_value) : arg_value;

    value_stack_.push(ir_builder.CreateCall(func_map[node.name_], {arg_value}));
}
