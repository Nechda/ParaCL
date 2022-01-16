#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace yy {
class parser;
}

namespace AST {

struct IVisitor;
using ExprVisitor = IVisitor;

struct ExprBase {
    virtual void dump() = 0;
    virtual bool is_ptr() { return false; }   /* Codegen return value */
    virtual bool is_brnch() { return false; } /* This expr is brach instr */

    virtual void on_visit(ExprVisitor *) = 0;
    virtual ~ExprBase() {}
};

struct ConstLineral : public ExprBase {
    ConstLineral(int val) : val_(val) {}
    void dump() override { std::cout << val_; }
    void on_visit(ExprVisitor *vi) override;
    ~ConstLineral() {}
    int val_;
};

struct BinaryInst : public ExprBase {
    enum OP { ADD, SUB, MUL, DIV, GT, LT, GE, LE, EQ, NE, UNDEFINED };

    BinaryInst(ExprBase *lhs, ExprBase *rhs, OP operation) : lhs_(lhs), rhs_(rhs), op_(operation) {}

    void dump() override {
        std::string op_s[] = {"+", "-", "*", "/", ">", "<", ">=", "<=", "==", "!="};
        lhs_->dump();
        std::cout << op_s[op_];
        rhs_->dump();
    }

    void on_visit(ExprVisitor *vi) override;
    ~BinaryInst() {}

    ExprBase *lhs_;
    ExprBase *rhs_;
    OP op_;
};

struct BranchInstBase : public ExprBase {
    enum BranchType { Uncondition, Condition };
    virtual BranchType get_type() = 0;
    bool is_brnch() override { return true; }
    ~BranchInstBase() {}
};

struct CondBranchInst : public BranchInstBase {
    CondBranchInst(ExprBase *cond, unsigned bb_for_cond, unsigned bb_next)
        : cond_(cond), bb_cond_(bb_for_cond), bb_next_(bb_next) {}
    void dump() override {
        std::cout << "branch node{\n";
        std::cout << "cond = ";
        cond_->dump();
        std::cout << std::endl;
        std::cout << "cond block = " << bb_cond_ << std::endl;
        std::cout << "next block = " << bb_next_ << std::endl;
        std::cout << "}";
    }

    void on_visit(ExprVisitor *vi) override;
    ~CondBranchInst() {}
    BranchType get_type() override { return BranchType::Condition; }

    ExprBase *cond_;
    unsigned bb_cond_ = -1;
    unsigned bb_next_ = -1;
};

struct BranchInst : public BranchInstBase {
    BranchInst(unsigned bb_next) : bb_next_(bb_next) {}
    void dump() override { std::cout << "Unconditional branch next block = " << bb_next_ << std::endl; }

    void on_visit(ExprVisitor *vi) override;
    ~BranchInst() {}
    BranchType get_type() override { return BranchType::Uncondition; }

    unsigned bb_next_ = -1;
};

struct Variable : public ExprBase {
    Variable(std::string name) : name_(name) {}
    void dump() override { std::cout << "Var = " << name_; }

    bool is_ptr() override { return true; } /* Codegen function return an addr */
    void on_visit(ExprVisitor *vi) override;
    ~Variable() {}

    std::string name_;
};

struct AssignInst : public ExprBase {
    AssignInst(ExprBase *lhs, ExprBase *rhs) : lhs_(lhs), rhs_(rhs) {}
    void dump() override {
        std::cout << "Assign lhs = {";
        lhs_->dump();
        std::cout << "} ";
        std::cout << "rhs = {";
        rhs_->dump();
        std::cout << "}";
    }

    void on_visit(ExprVisitor *vi) override;
    ~AssignInst() {}

    ExprBase *lhs_;
    ExprBase *rhs_;
};

struct FunctionCall : public ExprBase {
    FunctionCall(std::string name, ExprBase *arg) : name_(name), arg_(arg){};
    void dump() override {
        std::cout << "Function name = {" << name_ << "}\n";
        std::cout << "Arg = {";
        arg_->dump();
        std::cout << "}";
    }

    void on_visit(ExprVisitor *vi) override;
    ~FunctionCall() {}

    std::string name_;
    ExprBase *arg_;
};

struct Block {
    enum BranchReason { NONE, IF, WHILE };
    static unsigned total_blocks;
    Block() {}
    Block(std::vector<ExprBase *> lines_tmp, BranchReason br_reason)
        : ID(total_blocks), lines(std::move(lines_tmp)), reason(br_reason) {
        total_blocks++;
    }

    void dump() {
        std::cout << "===== Block " << ID << " =====" << std::endl;
        for (auto &it : lines) {
            std::cout << "New line\n";
            it->dump();
            std::cout << std::endl;
        }
    }
    unsigned ID = 0;
    std::vector<ExprBase *> lines;
    BranchReason reason;
};

struct ASTContext {
    std::vector<Block> blocks;
    std::vector<std::pair<int, int>> dom_edges;
    std::unordered_set<std::string> functions;

    ~ASTContext() {
        for (auto ptr : nodes)
            delete ptr;
    }

    void complete_cfg();
    void generate_dominator_tree();
    std::vector<int> create_dfs_order();
    std::vector<int> create_height_array();

  private:
    std::vector<ExprBase *> nodes;

  private:
    void rewrite_branches(unsigned cur_bb);

    friend class yy::parser;
};
} // namespace AST