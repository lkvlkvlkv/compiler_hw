#include "codeGen.hpp"

#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <vector>

#include "node.hpp"

void CodeGenContext::generateCode(NProgram& root) {
    std::cout << "Generating code...\n";
    root.codeGen(*this);
    return;
}

llvm::Value* NProgram::codeGen(CodeGenContext& context) {
    for (auto statement : statements) {
        statement->codeGen(context);
    }
    return nullptr;
}

llvm::Value* NExpression::codeGen(CodeGenContext& context) {
}

llvm::Value* NStatement::codeGen(CodeGenContext& context) {
}

llvm::Type* NType::codeGen(CodeGenContext& context) {
    return context.typeSystem.getVarType(name);
}

llvm::Value* NExpressionStatement::codeGen(CodeGenContext& context) {
    std::cout << "Generating code for " << expression->getType() << std::endl;
    return expression->codeGen(context);
}

llvm::Value* NBlock::codeGen(CodeGenContext& context) {
    std::cout << "Generating code for " << statements.size() << " statements\n";
    llvm::Value* last = nullptr;
    for (auto it = statements.begin(); it != statements.end(); it++) {
        last = (*it)->codeGen(context);
    }
    return last;
}

llvm::Value* NInteger::codeGen(CodeGenContext& context) {
    std::cout << "Creating integer: " << value << std::endl;
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context.llvmContext), value, true);
}

llvm::Value* NDouble::codeGen(CodeGenContext& context) {
    std::cout << "Creating double: " << value << std::endl;
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context.llvmContext), value);
}

llvm::Value* NIdentifier::codeGen(CodeGenContext& context) {
    llvm::Type* type = context.getSymbolType(name);
    llvm::Value* value = context.getSymbolValue(name);
    if (!value) {
        LogErrorV("Unknown variable name : " + name);
        return nullptr;
    }
    return context.builder.CreateLoad(type, value, name.c_str());
}

llvm::Value* NAssignment::codeGen(CodeGenContext& context) {
    std::cout << "Creating assignment for " << lhs->name << std::endl;
    llvm::Value* value = rhs->codeGen(context);
    context.builder.CreateStore(value, context.getSymbolValue(lhs->name));
    return value;
}

llvm::Value* NVariableDeclarationList::codeGen(CodeGenContext& context) {
    std::cout << "Creating variable declaration list\n";
    llvm::Value* last = nullptr;
    for (auto it = declarations.begin(); it != declarations.end(); it++) {
        last = (*it)->codeGen(context);
    }
    return last;
}

llvm::Value* NVariableDeclaration::codeGen(CodeGenContext& context) {
    std::cout << "Creating variable declaration of " << id->name << std::endl;
    llvm::Type* type = this->type->codeGen(context);
    llvm::Value* inst = context.builder.CreateAlloca(type, nullptr, this->id->name.c_str());

    if (assignmentExpr != nullptr) {
        llvm::Value* assignmentValue = assignmentExpr->codeGen(context);
        context.builder.CreateStore(assignmentValue, inst);
    }

    context.setSymbolType(id->name, this->type->codeGen(context));
    context.setSymbolValue(id->name, inst);

    return inst;
}

llvm::Value* NBinaryOperator::codeGen(CodeGenContext& context) {
    std::cout << "Creating binary operation " << op << std::endl;
    llvm::Value* left = lhs->codeGen(context);
    llvm::Value* right = rhs->codeGen(context);

    if (!left || !right) {
        return nullptr;
    }

    if (left->getType()->getTypeID() != right->getType()->getTypeID()) {
        LogErrorV("Type mismatch in binary operator");
        return nullptr;
    }

    switch (op) {
        case OP_PLUS:
            return context.builder.CreateAdd(left, right, "addtmp");
        case OP_MINUS:
            return context.builder.CreateSub(left, right, "subtmp");
        case OP_MULT:
            return context.builder.CreateMul(left, right, "multmp");
        case OP_DIV:
            return context.builder.CreateSDiv(left, right, "divtmp");
        case COM_LT:
            return context.builder.CreateICmpSLT(left, right, "lttmp");
        case COM_GT:
            return context.builder.CreateICmpSGT(left, right, "gttmp");
        case COM_EQ:
            return context.builder.CreateICmpEQ(left, right, "eqtmp");
        case COM_NE:
            return context.builder.CreateICmpNE(left, right, "netmp");
        case COM_LE:
            return context.builder.CreateICmpSLE(left, right, "letmp");
        case COM_GE:
            return context.builder.CreateICmpSGE(left, right, "getmp");
        default:
            LogErrorV("Invalid binary operator");
            return nullptr;
    }
}

llvm::Value* NFunctionDefinition::codeGen(CodeGenContext& context) {
    std::cout << "Creating function declaration for " << id->name << std::endl;
    std::vector<llvm::Type*> argTypes;
    for (auto it = arguments.begin(); it != arguments.end(); it++) {
        argTypes.push_back((*it)->type->codeGen(context));
    }
    llvm::FunctionType* functionType = llvm::FunctionType::get(type->codeGen(context), argTypes, false);
    llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, id->name.c_str(), context.theModule);

    return function;
}

llvm::Value* NFunctionDeclaration::codeGen(CodeGenContext& context) {
    std::cout << "Creating function definition for " << definition->id->name << std::endl;
    std::vector<llvm::Type*> argTypes;
    for (auto it = this->definition->arguments.begin(); it != this->definition->arguments.end(); it++) {
        argTypes.push_back((*it)->type->codeGen(context));
    }

    llvm::Function* function = context.theModule.getFunction(this->definition->id->name.c_str());

    if (!function) {
        llvm::FunctionType* functionType = llvm::FunctionType::get(this->definition->type->codeGen(context), argTypes, false);
        function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, this->definition->id->name.c_str(), context.theModule);
    }


    llvm::BasicBlock* block = llvm::BasicBlock::Create(context.llvmContext, "entry", function, 0);
    context.builder.SetInsertPoint(block);
    context.pushBlock(block);

    auto argIt = this->definition->arguments.begin();
    for (auto it = function->arg_begin(); it != function->arg_end(); it++) {
        it->setName((*argIt)->id->name.c_str());
        llvm::AllocaInst* inst = context.builder.CreateAlloca((*argIt)->type->codeGen(context), nullptr, (*argIt)->id->name.c_str());
        context.builder.CreateStore(&*it, inst);
        context.setSymbolValue((*argIt)->id->name, inst);
        context.setSymbolType((*argIt)->id->name, (*argIt)->type->codeGen(context));
        context.setFuncArg((*argIt)->id->name, true);
        argIt++;
    }

    this->block->codeGen(context);

    if (block->getTerminator() == nullptr) {
        if (function->getReturnType()->isVoidTy()) {
            context.setCurrentReturnValue(context.builder.CreateRetVoid());
        }
        else {
            LogErrorV("Non-void function must return a value");
        }
    }

    context.popBlock();
    llvm::verifyFunction(*function);
    return function;
}

llvm::Value* NReturnStatement::codeGen(CodeGenContext& context) {
    std::cout << "Creating return statement\n";
    if (this->expression == nullptr) {
        llvm::Value* returnValue = context.builder.CreateRetVoid();
        context.setCurrentReturnValue(returnValue);
        return returnValue;
    }
    llvm::Value* returnValue = this->expression->codeGen(context);
    context.setCurrentReturnValue(returnValue);
    return returnValue;
}

llvm::Value* NFunctionCall::codeGen(CodeGenContext& context) {
    std::cout << "Creating function call " << id->name << std::endl;
    llvm::Function* function = context.theModule.getFunction(id->name.c_str());
    if (!function) {
        return LogErrorV("Unknown function referenced");
    }

    if (function->arg_size() != arguments.size()) {
        return LogErrorV("Incorrect number of arguments passed");
    }

    std::vector<llvm::Value*> args;
    for (auto it = arguments.begin(); it != arguments.end(); it++) {
        args.push_back((*it)->codeGen(context));
        if (!args.back()) {
            return nullptr;
        }
    }

    return context.builder.CreateCall(function, args, "calltmp");
}

std::unique_ptr<NExpression> LogError(const char* str) {
    std::cout << "LogError: " << str << "\n"
              << std::endl;
    return nullptr;
}

llvm::Value* LogErrorV(std::string str) {
    return LogErrorV(str.c_str());
}

llvm::Value* LogErrorV(const char* str) {
    LogError(str);
    return nullptr;
}
