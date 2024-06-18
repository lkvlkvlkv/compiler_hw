#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>
#include "node.hpp"

llvm::Value* NExpression::codeGen(CodeGenContext& context) {

}

llvm::Value* NStatement::codeGen(CodeGenContext& context) {

}

llvm::Value* NExpressionStatement::codeGen(CodeGenContext& context) {

}

llvm::Value* NBlock::codeGen(CodeGenContext& context) {

}

llvm::Value* NInteger::codeGen(CodeGenContext& context) {

}

llvm::Value* NDouble::codeGen(CodeGenContext& context) {

}

llvm::Value* NIdentifier::codeGen(CodeGenContext& context) {

}

llvm::Value* NAssignment::codeGen(CodeGenContext& context) {

}

llvm::Value* NVariableDeclaration::codeGen(CodeGenContext& context) {

}

llvm::Value* NBinaryOperator::codeGen(CodeGenContext& context) {

}

llvm::Value* NFunctionDeclaration::codeGen(CodeGenContext& context) {

}

llvm::Value* NFunctionDefinition::codeGen(CodeGenContext& context) {

}

llvm::Value* NReturnStatement::codeGen(CodeGenContext& context) {

}

llvm::Value* NFunctionCall::codeGen(CodeGenContext& context) {

}