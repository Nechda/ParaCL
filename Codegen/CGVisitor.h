#pragma once

#include "AST/ASTVisitor.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <stack>

struct CodegenerationVisitor : public AST::IVisitor {
    CodegenerationVisitor(llvm::IRBuilder<>& builder) : ir_builder(builder) {}
    std::unordered_map<unsigned, llvm::BasicBlock*> bb_map;
    std::unordered_map<std::string, llvm::Function*> func_map;
    llvm::IRBuilder<>& ir_builder;

    void clear_value_stack();
    void height_align(int height);

    void visit(AST::ConstLineral& node);
    void visit(AST::BinaryInst& node);
    void visit(AST::CondBranchInst& node);
    void visit(AST::BranchInst& node);
    void visit(AST::Variable& node);
    void visit(AST::AssignInst& node);
    void visit(AST::FunctionCall& node);
       
  private:
    std::stack<llvm::Value*> value_stack_;
    std::vector<std::unordered_map<std::string, llvm::Value*>> blocks_variable;

    llvm::Value* search_variable(std::string& name);
};