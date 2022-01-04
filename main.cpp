
#include <iostream>
#include "Parser/Driver.hh"
#include "AST/AST.h"
#include "Codegen/Codegen.h"


auto GenerateAST() {
    Driver dr;
    int ret_code = dr.parse();
    return dr.result;
}

int main() {

    auto ast = GenerateAST();
    AST::complete_cfg(ast);

    llvm::LLVMContext ll_ctx;
    CG cg(ast, ll_ctx);
    auto module = std::move(cg.build_module());

    return 0;
}
