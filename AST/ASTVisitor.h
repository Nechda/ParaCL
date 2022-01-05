#include "AST/AST.h"

namespace AST {
    struct IVisitor {
        virtual void visit(ConstLineral& node) = 0;
        virtual void visit(BinaryInst& node) = 0;
        virtual void visit(CondBranchInst& node) = 0;
        virtual void visit(BranchInst& node) = 0;
        virtual void visit(Variable& node) = 0;
        virtual void visit(AssignInst& node) = 0;
        virtual void visit(FunctionCall& node) = 0;
    };
}