#include "AST/AST.h"
#include "Codegen/CGVisitor.h"

using namespace AST;

void ConstLineral::on_visit(ExprVisitor* vi) {vi->visit(*this);}
void BinaryInst::on_visit(ExprVisitor* vi) {vi->visit(*this);}
void CondBranchInst::on_visit(ExprVisitor* vi) {vi->visit(*this);}
void BranchInst::on_visit(ExprVisitor* vi) {vi->visit(*this);}
void Variable::on_visit(ExprVisitor* vi) {vi->visit(*this);}
void AssignInst::on_visit(ExprVisitor* vi) {vi->visit(*this);}
void FunctionCall::on_visit(ExprVisitor* vi) {vi->visit(*this);}
