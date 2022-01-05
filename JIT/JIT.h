#pragma once

#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/IndirectionUtils.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/SpeculateAnalyses.h"
#include "llvm/ExecutionEngine/Orc/Speculation.h"
#include "llvm/ExecutionEngine/Orc/ObjectTransformLayer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ThreadPool.h"

#include "llvm/Transforms/Utils/SplitModule.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"

#include "Runtime/runtime.h"

namespace llvm { namespace orc {
    class JIT {
      public:
        static Expected<std::unique_ptr<JIT>> Create() {
            auto JTMB = orc::JITTargetMachineBuilder::detectHost();
            if (!JTMB)
                return JTMB.takeError();
            JTMB->setCodeGenOptLevel(CodeGenOpt::Default);

            auto DL = JTMB->getDefaultDataLayoutForTarget();
            if (!DL)
              return DL.takeError();

            auto ES = std::make_unique<ExecutionSession>();

            auto ProcessSymbolsSearchGenerator =
                DynamicLibrarySearchGenerator::GetForCurrentProcess(DL->getGlobalPrefix());
            if (!ProcessSymbolsSearchGenerator)
                return ProcessSymbolsSearchGenerator.takeError();

            std::unique_ptr<JIT>
                jit(
                     new JIT(
                         std::move(ES), std::move(*DL), std::move(*JTMB),
                         std::move(*ProcessSymbolsSearchGenerator)
                     )
                );
            return std::move(jit);
        }

        ExecutionSession &getES() {return *ES; };
        Error addModule(ThreadSafeModule& TSM) { return CompileLayer.add(MainJD, std::move(TSM)); }
        Expected<JITEvaluatedSymbol> lookup(StringRef name) { return ES->lookup({&MainJD}, Mangle(name)); }
      private:
        /* Constructor */
        JIT(
            std::unique_ptr<ExecutionSession> ES,
            DataLayout DL,
            orc::JITTargetMachineBuilder JTMB,
            std::unique_ptr<DynamicLibrarySearchGenerator> ProcessSymbolsGenerator
        ) :
            ES(std::move(ES)),
            DL(std::move(DL)),
            Mangle(*this->ES, this->DL),
            MainJD(this->ES->createJITDylib("<main>")),
            ObjLayer(*this->ES, createMM),
            CompileLayer(*this->ES, ObjLayer, std::make_unique<ConcurrentIRCompiler>(std::move(JTMB)))
        {
            MainJD.addGenerator(std::move(ProcessSymbolsGenerator));

            llvm::JITSymbolFlags functionFlags;
            functionFlags |= llvm::JITSymbolFlags::Callable;
            functionFlags |= llvm::JITSymbolFlags::Exported;
            functionFlags |= llvm::JITSymbolFlags::Absolute;

            llvm::orc::SymbolMap runtimeSymbols;
            runtimeSymbols[Mangle("print")] = llvm::JITEvaluatedSymbol(reinterpret_cast<llvm::JITTargetAddress>(print), functionFlags);

            MainJD.define(llvm::orc::absoluteSymbols(runtimeSymbols));
        }

      private:

        static std::unique_ptr<SectionMemoryManager> createMM() { return std::make_unique<SectionMemoryManager>(); }

      private:

        /* Interanl structures */
        std::unique_ptr<ExecutionSession> ES;
        DataLayout DL;
        MangleAndInterner Mangle;
        JITDylib &MainJD;
 
        /* Compilator layers */
        RTDyldObjectLinkingLayer ObjLayer;
        IRCompileLayer CompileLayer; 
    };

    void execute_module(ThreadSafeModule TSM) {
        auto jit = cantFail(JIT::Create());
        jit->addModule(TSM);

        auto main_func_sym = cantFail(jit->lookup("main"));
        auto main_func = jitTargetAddressToFunction<void (*)()>(main_func_sym.getAddress());

        main_func();
    }
}}

