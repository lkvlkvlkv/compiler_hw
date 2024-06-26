#pragma once

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <map>

#include "node.hpp"

class TypeSystem{
private:
    llvm::LLVMContext &llvmContext;

    std::map<llvm::Type*, std::map<llvm::Type*, llvm::CastInst::CastOps>> _castTable;

    void addCast(llvm::Type* from, llvm::Type* to, llvm::CastInst::CastOps op);

public:
    llvm::Type* intTy = llvm::Type::getInt64Ty(llvmContext);
    llvm::Type* doubleTy = llvm::Type::getDoubleTy(llvmContext);
    llvm::Type* voidTy = llvm::Type::getVoidTy(llvmContext);

    TypeSystem(llvm::LLVMContext &context): llvmContext(context){
        addCast(intTy, doubleTy, llvm::CastInst::SIToFP);
        addCast(doubleTy, intTy, llvm::CastInst::FPToSI);
    }

    llvm::Type* getVarType(const NIdentifier& type) const;
    llvm::Type* getVarType(std::string typeStr) const; 
};