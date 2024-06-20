#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/IR/LegacyPassManager.h>

#include "CodeGen.hpp"
#include "ObjGen.hpp"

void ObjGen(CodeGenContext &context, const std::string &filename) {
    // Initialize the target registry etc.
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    context.theModule.setTargetTriple(targetTriple);

    std::string error;
    auto Target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    
    if (!Target) {
        llvm::errs() << error;
        return;
    }

    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Reloc::Model::PIC_; // Use Position Independent Code model
    auto theTargetMachine = Target->createTargetMachine(targetTriple, CPU, features, opt, RM);
    context.theModule.setDataLayout(theTargetMachine->createDataLayout());
    context.theModule.setTargetTriple(targetTriple);

    std::error_code EC;
    std::string targetFilename = filename + ".o";
    llvm::raw_fd_ostream dest(targetFilename, EC, llvm::sys::fs::OF_None);
    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }
    
    llvm::legacy::PassManager pass;
    auto fileType = llvm::CodeGenFileType::ObjectFile;
    if (theTargetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return;
    }
    context.theModule.print(llvm::errs(), nullptr);
    pass.run(context.theModule);
    dest.flush();

    std::string llFilename = filename + ".ll";
    llvm::raw_fd_ostream llDest(llFilename, EC, llvm::sys::fs::OF_None);
    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }

    context.theModule.print(llDest, nullptr);
    llDest.flush();
}
