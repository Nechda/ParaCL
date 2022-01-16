#include "Interpret/Interpret.h"
#include "AST/ASTVisitor.h"
#include "Runtime/runtime.h"

#include <stack>
#include <string>
#include <unordered_map>

#define ASSERT(expr_)                                                                                                  \
    if (!(expr_)) {                                                                                                    \
        printf("Assert failed: " #expr_);                                                                              \
        exit(1);                                                                                                       \
    }

struct ExecutionVisitor : public AST::IVisitor {

    ExecutionVisitor(AST::ASTContext &ast) : ast_(ast) {}

    void execute() {
        const auto last_bb_id = ast_.blocks.size();
        while (last_bb_id != cur_bb_) {
            execute_block(cur_bb_);
        }
    }

    void visit(AST::ConstLineral &node) { value_stack_.push(node.val_); }

    void visit(AST::BinaryInst &node) {
        node.rhs_->on_visit(this);
        auto rhs_value = value_stack_.top();
        value_stack_.pop();

        node.lhs_->on_visit(this);
        auto lhs_value = value_stack_.top();
        value_stack_.pop();

        switch (node.op_) {
        case AST::BinaryInst::OP::ADD:
            value_stack_.push(lhs_value + rhs_value);
            break;
        case AST::BinaryInst::OP::SUB:
            value_stack_.push(lhs_value - rhs_value);
            break;
        case AST::BinaryInst::OP::MUL:
            value_stack_.push(lhs_value * rhs_value);
            break;
        case AST::BinaryInst::OP::DIV:
            value_stack_.push(lhs_value / rhs_value);
            break;
        default:
            break;
        }
    }

    void visit(AST::CondBranchInst &node) {
        node.cond_->on_visit(this);
        auto cond_value = value_stack_.top();
        value_stack_.pop();

        cur_bb_ = !!cond_value ? node.bb_cond_ : node.bb_next_;
    }

    void visit(AST::BranchInst &node) { cur_bb_ = node.bb_next_; }

    void visit(AST::Variable &node) { value_stack_.push(variables_[node.name_]); }

    void visit(AST::AssignInst &node) {
        ASSERT(node.lhs_->is_ptr());
        auto &variable_node = *reinterpret_cast<AST::Variable *>(node.lhs_);

        node.rhs_->on_visit(this);
        auto rhs_value = value_stack_.top();
        value_stack_.pop();

        variables_[variable_node.name_] = rhs_value;

        value_stack_.push(rhs_value);
    }

    void visit(AST::FunctionCall &node) {
        /* Support only runtime functions */

        node.arg_->on_visit(this);
        auto arg_value = value_stack_.top();
        value_stack_.pop();

        // TODO: create runtime interface for it
        if (node.name_ == "print") {
            print(arg_value);
        } else {
            ASSERT(!"This function is not supported.");
        }
    }

  private:
    void execute_block(unsigned cur_bb_id) {
        auto &bb = ast_.blocks[cur_bb_id];
        auto &lines = bb.lines;

        for (auto &expr : lines) {
            expr->on_visit(this);
            value_stack_ = {};
        }
    }

    unsigned cur_bb_ = 0;
    std::unordered_map<std::string, int> variables_;
    std::stack<int> value_stack_;

    AST::ASTContext &ast_;
};

void Interpretator::run() {
    ExecutionVisitor EV(ast_);
    EV.execute();
}