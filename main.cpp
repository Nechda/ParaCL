
#include "AST/AST.h"
#include "Codegen/Codegen.h"
#include "Interpret/Interpret.h"
#include "JIT/JIT.h"
#include "Parser/Driver.hh"
#include <iostream>

int main(int argc, char **argv) {
    std::unique_ptr<AST::ASTContext> ast;
    try {
        ast = Driver().parse();
        ast->complete_cfg();
        ast->generate_dominator_tree();
    } catch (const std::exception &e) {
        std::cout << "[[FRONT]] Catch unexpected exception:\n";
        std::cout << e.what() << std::endl;
        return 1;
    }

    std::unique_ptr<llvm::LLVMContext> ll_ctx;
    std::unique_ptr<llvm::Module> module;
    try {
        ll_ctx = std::make_unique<llvm::LLVMContext>();
        module = CG(*ast, *ll_ctx).build_module();
    } catch (const std::exception &e) {
        std::cout << "[[CG]] Catch unexpected exception:\n";
        std::cout << e.what() << std::endl;
        return 1;
    }

#if 1

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::orc::execute_module(llvm::orc::ThreadSafeModule(std::move(module), std::move(ll_ctx)));

#else

    Interpretator interpret(ast);
    interpret.run();

#endif

    return 0;
}
