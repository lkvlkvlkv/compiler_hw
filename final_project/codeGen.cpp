#include "codeGen.hpp"

#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>

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
    if (!value) {
        return nullptr;
    }
    llvm::Value* lhsValue = context.getSymbolValue(lhs->name);
    if (!lhsValue) {
        LogErrorV("Unknown variable name : " + lhs->name);
        return nullptr;
    }
    return context.builder.CreateStore(value, lhsValue);
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
    if (context.isGlobal()) {
        // global variable
        std::cout << "Creating global variable declaration of " << id->name << std::endl;
        llvm::Type* type = this->type->codeGen(context);

        context.theModule.getOrInsertGlobal(id->name, type);
        llvm::GlobalVariable* global = context.theModule.getNamedGlobal(id->name);
        global->setLinkage(llvm::GlobalValue::CommonLinkage);

        if (assignmentExpr != nullptr) {
            llvm::Value* assignmentValue = assignmentExpr->codeGen(context);
            global->setInitializer(llvm::dyn_cast<llvm::Constant>(assignmentValue));
        }
        else {
            global->setInitializer(llvm::Constant::getNullValue(type));
        }

        context.setSymbolType(id->name, type);
        context.setSymbolValue(id->name, global);    
        return global;
    }

    // local variable
    std::cout << "Creating variable declaration of " << id->name << std::endl;
    llvm::Type* type = this->type->codeGen(context);
    llvm::Value* inst = context.builder.CreateAlloca(type, nullptr, this->id->name.c_str());

    if (assignmentExpr != nullptr) {
        llvm::Value* assignmentValue = assignmentExpr->codeGen(context);
        context.builder.CreateStore(assignmentValue, inst);
    }

    context.setSymbolType(id->name, type);
    context.setSymbolValue(id->name, inst);
    return inst;
}

llvm::Value* NBinaryOperator::codeGen(CodeGenContext& context) {
    std::cout << "Creating binary operation " << op << std::endl;
    llvm::Value* left = lhs->codeGen(context);
    llvm::Value* right = rhs->codeGen(context);
    bool fp = false;

    if (!left || !right) {
        return nullptr;
    }

    if (left->getType()->getTypeID() != right->getType()->getTypeID()) {
        LogErrorV("Type mismatch in binary operator");
        return nullptr;
    }

    if( (left->getType()->getTypeID() == llvm::Type::DoubleTyID) || (right->getType()->getTypeID() == llvm::Type::DoubleTyID) ){  // type upgrade
        fp = true;
        if( (right->getType()->getTypeID() != llvm::Type::DoubleTyID) ){
            left = context.builder.CreateUIToFP(right, llvm::Type::getDoubleTy(context.llvmContext), "ftmp");
        }
        if( (left->getType()->getTypeID() != llvm::Type::DoubleTyID) ){
            left = context.builder.CreateUIToFP(left, llvm::Type::getDoubleTy(context.llvmContext), "ftmp");
        }
    }

    std::string type_str1, type_str2;
    llvm::raw_string_ostream rso1(type_str1), rso2(type_str2);
    left->getType()->print(rso1);
    right->getType()->print(rso2);
    std::cout << "Type1: " << rso1.str() << " Type2: " << rso2.str() << std::endl;

    switch (op) {
        case OP_PLUS:
            return fp? context.builder.CreateFAdd(left, right, "addtmp") : context.builder.CreateAdd(left, right, "addtmp");
        case OP_MINUS:
            return fp? context.builder.CreateFSub(left, right, "subtmp") : context.builder.CreateSub(left, right, "subtmp");
        case OP_MULT:
            return fp? context.builder.CreateFMul(left, right, "multmp") : context.builder.CreateMul(left, right, "multmp");
        case OP_DIV:
            return fp? context.builder.CreateFDiv(left, right, "divtmp") : context.builder.CreateSDiv(left, right, "divtmp");
        case COM_LT:
            return fp? context.builder.CreateFCmpOLT(left, right, "lttmp") : context.builder.CreateICmpSLT(left, right, "lttmp");
        case COM_GT:
            return fp? context.builder.CreateFCmpOGT(left, right, "gttmp") : context.builder.CreateICmpSGT(left, right, "gttmp");
        case COM_EQ:
            return fp? context.builder.CreateFCmpOEQ(left, right, "eqtmp") : context.builder.CreateICmpEQ(left, right, "eqtmp");
        case COM_NE:
            return fp? context.builder.CreateFCmpONE(left, right, "netmp") : context.builder.CreateICmpNE(left, right, "netmp");
        case COM_LE:
            return fp? context.builder.CreateFCmpOLE(left, right, "letmp") : context.builder.CreateICmpSLE(left, right, "letmp");
        case COM_GE:
            return fp? context.builder.CreateFCmpOGE(left, right, "getmp") : context.builder.CreateICmpSGE(left, right, "getmp");
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

llvm::Value* NIfStatement::codeGen(CodeGenContext& context) {
    std::cout << "Creating if statement\n";
    llvm::Value* conditionValue = condition->codeGen(context);
    if (!conditionValue) {
        return nullptr;
    }

    llvm::Function* function = context.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(context.llvmContext, "then", function);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context.llvmContext, "else", function);
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context.llvmContext, "mergeBlock", function);

    if (this->elseBlock == nullptr) {
        context.builder.CreateCondBr(conditionValue, thenBlock, mergeBlock);
    } else {
        context.builder.CreateCondBr(conditionValue, thenBlock, elseBlock);
    }

    context.builder.SetInsertPoint(thenBlock);
    context.pushBlock(thenBlock);

    this->thenBlock->codeGen(context);

    context.popBlock();
    
    thenBlock = context.builder.GetInsertBlock();

    if (thenBlock->getTerminator() == nullptr) {
        context.builder.CreateBr(mergeBlock);
    }

    if (this->elseBlock) {
        context.builder.SetInsertPoint(elseBlock);

        context.pushBlock(elseBlock);

        this->elseBlock->codeGen(context);

        context.popBlock();

        elseBlock = context.builder.GetInsertBlock();

        if (elseBlock->getTerminator() == nullptr) {
            context.builder.CreateBr(mergeBlock);
        }
    }

    context.builder.SetInsertPoint(mergeBlock);

    return nullptr;
}

llvm::Value* NWhileStatement::codeGen(CodeGenContext& context) {
    std::cout << "Creating while statement\n";
    llvm::Function* function = context.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(context.llvmContext, "while_condition", function);
    llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(context.llvmContext, "while_loop", function);
    llvm::BasicBlock* afterBlock = llvm::BasicBlock::Create(context.llvmContext, "while_after", function);

    context.builder.CreateBr(conditionBlock);
    context.builder.SetInsertPoint(conditionBlock);
    context.pushBlock(conditionBlock);

    llvm::Value* conditionValue = condition->codeGen(context);
    std::cout << "Condition value: " << conditionValue << std::endl;
    if (!conditionValue) {
        std::cout << "Condition value is null\n";
        return nullptr;
    }

    context.builder.CreateCondBr(conditionValue, loopBlock, afterBlock);

    context.builder.SetInsertPoint(loopBlock);
    context.pushBlock(loopBlock);
    context.setBreakStack(afterBlock);
    context.setContinueStack(conditionBlock);

    this->block->codeGen(context);

    context.popBlock();

    loopBlock = context.builder.GetInsertBlock();

    if (loopBlock->getTerminator() == nullptr) {
        context.builder.CreateBr(conditionBlock);
    }

    context.builder.SetInsertPoint(afterBlock);

    context.popContinueStack();
    context.popBreakStack();
    return nullptr;
}

llvm::Value* NForStatement::codeGen(CodeGenContext& context) {
    std::cout << "Creating for statement\n";
    llvm::Function* function = context.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* initBlock = llvm::BasicBlock::Create(context.llvmContext, "for_init", function);
    llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(context.llvmContext, "for_condition", function);
    llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(context.llvmContext, "for_loop", function);
    llvm::BasicBlock* incrementBlock = llvm::BasicBlock::Create(context.llvmContext, "for_increment", function);
    llvm::BasicBlock* afterBlock = llvm::BasicBlock::Create(context.llvmContext, "for_after", function);

    context.builder.CreateBr(initBlock);
    context.builder.SetInsertPoint(initBlock);
    init->codeGen(context);
    context.builder.CreateBr(conditionBlock);

    context.builder.SetInsertPoint(conditionBlock);
    llvm::Value* conditionValue = condition->codeGen(context);
    if (!conditionValue) {
        std::cout << "Condition value is null\n";
        return nullptr;
    }
    context.builder.CreateCondBr(conditionValue, loopBlock, afterBlock);

    context.builder.SetInsertPoint(loopBlock);
    context.pushBlock(loopBlock);
    context.setBreakStack(afterBlock);
    context.setContinueStack(incrementBlock);
    this->block->codeGen(context);
    context.popBlock();

    loopBlock = context.builder.GetInsertBlock();
    if (loopBlock->getTerminator() == nullptr) {
        context.builder.CreateBr(incrementBlock);
    }

    context.builder.SetInsertPoint(incrementBlock);
    increment->codeGen(context);
    context.builder.CreateBr(conditionBlock);

    context.builder.SetInsertPoint(afterBlock);
    context.popBreakStack();
    context.popContinueStack();
    return nullptr;
}

llvm::Value* NContinueStatement::codeGen(CodeGenContext& context) {
    std::cout << "Creating continue statement\n";
    llvm::BasicBlock* loopBlock = context.getContinueStack();
    if (loopBlock == nullptr) {
        return LogErrorV("Continue statement not in loop");
    }
    context.builder.CreateBr(loopBlock);
    return nullptr;
}

llvm::Value* NBreakStatement::codeGen(CodeGenContext& context) {
    std::cout << "Creating break statement\n";
    llvm::BasicBlock* loopBlock = context.getBreakStack();
    if (loopBlock == nullptr) {
        return LogErrorV("Break statement not in loop");
    }
    context.builder.CreateBr(loopBlock);
    return nullptr;
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
