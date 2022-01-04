#pragma once

#include "AST/AST.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

#include <unordered_map>
#include <string>
#include <stack>

struct CodegenerationVisitor {
    CodegenerationVisitor(llvm::IRBuilder<>& builder) : ir_builder(builder) {}
    std::unordered_map<std::string, llvm::Value*> named_value;
    std::unordered_map<unsigned, llvm::BasicBlock*> bb_map;
    std::unordered_map<std::string, llvm::Function*> func_map;
    llvm::IRBuilder<>& ir_builder;

    void clear_value_stack();

    void visit(AST::ConstLineral& node);
    void visit(AST::BinaryInst& node);
    void visit(AST::CondBranchInst& node);
    void visit(AST::BranchInst& node);
    void visit(AST::Variable& node);
    void visit(AST::AssignInst& node);
    void visit(AST::FunctionCall& node);
       
  private:
    std::stack<llvm::Value*> value_stack_;
};