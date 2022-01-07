
#include <iostream>
#include "Parser/Driver.hh"
#include "AST/AST.h"
#include "Codegen/Codegen.h"
#include "JIT/JIT.h"
#include "Interpret/Interpret.h"

#include <vector>


auto GenerateAST() {
    Driver dr;
    int ret_code = dr.parse();
    return dr.result;
}



int main(int argc, char** argv) {

    llvm::InitLLVM X(argc, argv);
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto ast = GenerateAST();
    AST::complete_cfg(ast);
    AST::generate_dominator_tree(ast);

    #if 0

    std::unique_ptr<llvm::LLVMContext> ll_ctx = std::make_unique<llvm::LLVMContext>();
    CG cg(ast, *ll_ctx);
    auto module = std::move(cg.build_module());

    llvm::orc::execute_module(llvm::orc::ThreadSafeModule(std::move(module), std::move(ll_ctx)));

    #else 

    Interpretator interpret(ast);
    interpret.run();

    #endif

    return 0;
}
