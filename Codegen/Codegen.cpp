#include "Codegen/Codegen.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/Support/raw_ostream.h"

using llvm::LLVMContext;
using llvm::Module;
using llvm::Function;
using llvm::FunctionType;
using llvm::BasicBlock;


Function* CG::define_functions(Module& module) {
    /* define void main() */
    auto func_ty = FunctionType::get(ir_builder_.getVoidTy(), false);
    auto main_fu = Function::Create(func_ty, Function::ExternalLinkage, "main", &module);

    /* declare other functions */
    auto& functions = ast_.functions;
    for(auto& func_name : functions) {
        auto void_ty = ir_builder_.getVoidTy();
        auto i32_ty = ir_builder_.getInt32Ty();
        auto ext_func_ty = FunctionType::get(ir_builder_.getVoidTy(), {i32_ty}, false);
        auto ext_func = Function::Create(ext_func_ty, Function::ExternalLinkage, func_name, &module);
        ext_func->deleteBody();
        cg_visitor.func_map[func_name] = ext_func;
    }

    return main_fu;
}

void CG::define_blocks(Function& func) {
    /* Create special blocks in main func */
    auto& blocks = ast_.blocks;
    auto entry_bb = cg_visitor.bb_map[blocks.size()+1] = BasicBlock::Create(ll_ctx_, "entry_bb", &func);

    auto last_bb = cg_visitor.bb_map[blocks.size()] = BasicBlock::Create(ll_ctx_, "last_bb", &func);
    ir_builder_.SetInsertPoint(last_bb);
    ir_builder_.CreateRetVoid();

    /* Walk around AST-blocks and create it in LLVM IR */
    for(auto& ast_bb : blocks) {
        auto id = ast_bb.ID;
        auto bb_name = std::string("BB_") + std::to_string(id);
        auto llvm_bb = BasicBlock::Create(ll_ctx_, bb_name, &func);

        /* Insert llvm bb into map of blocks */
        cg_visitor.bb_map[id] = llvm_bb;
    }

    /* Hack for variables */
    auto& variables = ast_.variables;
    ir_builder_.SetInsertPoint(entry_bb);
    for(auto& var_name : variables) {
        std::string tmp = var_name + "_ptr";
        auto var_ptr = ir_builder_.CreateAlloca(ir_builder_.getInt32Ty(), 0, tmp);
        cg_visitor.named_value[var_name] = var_ptr;
    }

    /* Jmp to the actual entry block */
    ir_builder_.CreateBr(cg_visitor.bb_map[blocks.size() - 1]);
}

void CG::build_blocks() {
    /* Generate LLVM IR for each ast-block */
    auto& blocks = ast_.blocks;
    for(auto& ast_bb : blocks) {
        auto id = ast_bb.ID;
        ir_builder_.SetInsertPoint(cg_visitor.bb_map[id]);
        for(auto& line : ast_bb.lines) {
            line->on_visit(&cg_visitor);
            cg_visitor.clear_value_stack();
        }
    }
}

void CG::optimize_module(Module& module) {
    /* CFG simplification for better IR reading */
    llvm::legacy::FunctionPassManager FPM(&module);
    FPM.add(llvm::createCFGSimplificationPass());
    for(auto& f : module)
        FPM.run(f);
}

void CG::dump_module(Module& module) {
    /* Dump generated module */
    std::string out_str;
    llvm::raw_string_ostream os(out_str);
    module.print(os, nullptr);
    os.flush();
    printf("#[LLVM IR]:\n");
    printf("%s", out_str.c_str());
    printf("#[=======]\n");
}

std::unique_ptr<Module> CG::build_module() {
    auto main_module = std::make_unique<Module>("Main", ll_ctx_);
    auto main_fu = define_functions(*main_module);
    define_blocks(*main_fu);
    build_blocks();
    optimize_module(*main_module);
    dump_module(*main_module);
    return std::move(main_module);
}