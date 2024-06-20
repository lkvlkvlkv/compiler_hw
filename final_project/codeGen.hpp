#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "node.hpp"
#include "grammar.hpp"
#include "typeSystem.hpp"

class CodeGenBlock {
public:
    llvm::BasicBlock* block;
    llvm::Value* returnValue;
    std::map<std::string, llvm::Value*> locals;
    std::map<std::string, llvm::Type*> types;
    std::map<std::string, bool> isFuncArg;
};

class CodeGenContext {
private:
    std::vector<CodeGenBlock*> blockStack;

public:
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder;
    llvm::Module theModule;
    std::map<std::string, llvm::Value*> globalVars;
    std::map<std::string, llvm::Type*> globalVarsType;
    TypeSystem typeSystem;

    CodeGenContext() : builder(llvmContext), typeSystem(llvmContext), theModule("main", this->llvmContext) {}

    llvm::Value* getSymbolValue(std::string name) {
        for (auto it = blockStack.rbegin(); it != blockStack.rend(); it++) {
            if ((*it)->locals.find(name) != (*it)->locals.end()) {
                return (*it)->locals[name];
            }
        }
        if (globalVars.find(name) != globalVars.end()) {
            return globalVars[name];
        }
        std::cout << "Unknown variable name: " << name << std::endl;
        return nullptr;
    }

    llvm::Type* getSymbolType(std::string name) {
        for (auto it = blockStack.rbegin(); it != blockStack.rend(); it++) {
            if ((*it)->types.find(name) != (*it)->types.end()) {
                return (*it)->types[name];
            }
        }
        if (globalVarsType.find(name) != globalVarsType.end()) {
            return globalVarsType[name];
        }
        std::cout << "Unknown variable name: " << name << std::endl;
        return nullptr;
    }

    bool isFuncArg(std::string name) const {
        for (auto it = blockStack.rbegin(); it != blockStack.rend(); it++) {
            if ((*it)->isFuncArg.find(name) != (*it)->isFuncArg.end()) {
                return (*it)->isFuncArg[name];
            }
        }
        return false;
    }

    bool isGlobal() const {
        return blockStack.size() == 0;
    }

    void setSymbolValue(std::string name, llvm::Value* value) {
        if (isGlobal()) {
            globalVars[name] = value;
            return;
        }
        blockStack.back()->locals[name] = value;
    }

    void setSymbolType(std::string name, llvm::Type* type) {
        if (isGlobal()) {
            globalVarsType[name] = type;
            return;
        }
        blockStack.back()->types[name] = type;
    }

    void setFuncArg(std::string name, bool value) {
        blockStack.back()->isFuncArg[name] = value;
    }

    llvm::BasicBlock* currentBlock() {
        return blockStack.back()->block;
    }

    void pushBlock(llvm::BasicBlock* block) {
        CodeGenBlock* codeGenBlock = new CodeGenBlock();
        codeGenBlock->block = block;
        codeGenBlock->returnValue = nullptr;
        blockStack.push_back(codeGenBlock);
    }

    void popBlock() {
        CodeGenBlock* codeGenBlock = blockStack.back();
        blockStack.pop_back();
        delete codeGenBlock;
    }

    void setCurrentReturnValue(llvm::Value* value) {
        blockStack.back()->returnValue = value;
        builder.CreateRet(value);
    }

    llvm::Value* getCurrentReturnValue() {
        return blockStack.back()->returnValue;
    }

    void generateCode(NProgram& root);
};

llvm::Value* LogErrorV(const char* str);
llvm::Value* LogErrorV(std::string str);
