#pragma once

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

#include "AST/AST.h"
#include "Codegen/CGVisitor.h"

class CG {
  public:
    CG(AST::ASTContext &ast, llvm::LLVMContext &ll_ctx)
        : ast_(ast), ll_ctx_(ll_ctx), ir_builder_(ll_ctx_), cg_visitor(ir_builder_) {}

    std::unique_ptr<llvm::Module> build_module();

    static void dump_module(const llvm::Module &module);

  private:
    AST::ASTContext &ast_;
    llvm::LLVMContext &ll_ctx_;
    llvm::IRBuilder<> ir_builder_;
    CodegenerationVisitor cg_visitor;

    llvm::Function *define_functions(llvm::Module &module); /* returns ptr to main */
    void define_blocks(llvm::Function &func);
    void build_blocks();
    void optimize_module(llvm::Module &module);
};