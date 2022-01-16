
#include "AST/AST.h"
#include "Codegen/Codegen.h"
#include "Interpret/Interpret.h"
#include "JIT/JIT.h"
#include "Parser/Driver.h"
#include <iostream>

#include "llvm/Support/CommandLine.h"

enum ExecMode { jit, interpretator };
llvm::cl::opt<ExecMode> exec_mode(llvm::cl::desc("Choose execution mode:"),
                                  llvm::cl::values(clEnumVal(jit, "Use llvm's jit for execution"),
                                                   clEnumVal(interpretator, "Use interpretator for execution")));
llvm::cl::opt<bool> dump_module("dump-module", llvm::cl::desc("Dump llvm module to cerr"));

int main(int argc, char **argv) {
    llvm::cl::ParseCommandLineOptions(argc, argv);

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

    if (dump_module) {
        CG::dump_module(*module);
    }

    if (exec_mode == jit) {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::orc::execute_module(llvm::orc::ThreadSafeModule(std::move(module), std::move(ll_ctx)));
    }
    if (exec_mode == interpretator) {
        Interpretator interpret(*ast);
        interpret.run();
    }

    return 0;
}
