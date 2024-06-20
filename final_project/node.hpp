#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <llvm/IR/Value.h>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

class Node {
public:
    // virtual ~Node() {}
    virtual llvm::Value* codeGen(CodeGenContext& context) = 0;
    virtual std::string getType() { return "Node"; }
};

class NProgram : public Node {
public:
    std::vector<std::shared_ptr<NStatement>> statements;
    NProgram() {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "Program"; }
};

class NType {
public:
    std::string name;
    NType(std::string name) : name(name) {}
    virtual llvm::Type* codeGen(CodeGenContext& context);
    virtual std::string getType() { return "NType"; }
};

class NExpression : public Node {
public:
    NExpression(){}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NExpression"; }
};

class NStatement : public Node {
public:
    NStatement(){}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NStatement"; }
};

class NExpressionStatement : public NStatement {
public:
	std::shared_ptr<NExpression> expression;

	NExpressionStatement(std::shared_ptr<NExpression> expression)
		: expression(expression) {
	}

	virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NExpressionStatement"; }
};

class NBlock : public NStatement {
public:
    std::vector<NStatement*> statements;
    NBlock() {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NBlock"; }
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NInteger"; }
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NDouble"; }
};

class NIdentifier : public NExpression
{
public:
    std::string name;
    NIdentifier(std::string name) : name(name) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NIdentifier"; }
};

class NAssignment : public NExpression {
public:
    std::shared_ptr<NIdentifier> lhs;
    std::shared_ptr<NExpression> rhs;
    NAssignment(std::shared_ptr<NIdentifier> lhs, std::shared_ptr<NExpression> rhs) : lhs(lhs), rhs(rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NAssignment"; }
};

class NVariableDeclarationList : public NExpression {
public:
    std::vector<std::shared_ptr<NVariableDeclaration>> declarations;
    NVariableDeclarationList(std::vector<std::shared_ptr<NVariableDeclaration>> declarations) : declarations(declarations) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NVariableDeclarationList"; }
};

class NVariableDeclaration : public NExpression {
public:
    std::shared_ptr<NType> type;
    std::shared_ptr<NIdentifier> id;
    std::shared_ptr<NExpression> assignmentExpr;
    NVariableDeclaration(std::shared_ptr<NType> type, std::shared_ptr<NIdentifier> id, std::shared_ptr<NExpression> assignmentExpr = nullptr) : type(type), id(id), assignmentExpr(assignmentExpr) {}
    NVariableDeclaration(std::shared_ptr<NType> type, std::shared_ptr<NAssignment> assignment) : type(type), id(assignment->lhs), assignmentExpr(assignment->rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NVariableDeclaration"; }
};

class NBinaryOperator : public NExpression {
public:
    int op;
    std::shared_ptr<NExpression> lhs;
    std::shared_ptr<NExpression> rhs;
    NBinaryOperator(std::shared_ptr<NExpression> lhs, int op, std::shared_ptr<NExpression> rhs) : lhs(lhs), op(op), rhs(rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NBinaryOperator"; }
};

class NFunctionDefinition : public NStatement {
public:
    std::shared_ptr<NType> type;
    std::shared_ptr<NIdentifier> id;
    std::vector<std::shared_ptr<NVariableDeclaration>> arguments;
    NFunctionDefinition(std::shared_ptr<NType> type, std::shared_ptr<NIdentifier> id, std::vector<std::shared_ptr<NVariableDeclaration>> arguments) : type(type), id(id), arguments(arguments) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NFunctionDeclaration"; }
};

class NFunctionDeclaration : public NStatement {
public:
    std::shared_ptr<NFunctionDefinition> definition;
    std::shared_ptr<NBlock> block;
    NFunctionDeclaration(std::shared_ptr<NFunctionDefinition> definition, std::shared_ptr<NBlock> block) : definition(definition), block(block) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NFunctionDefinition"; }
};

class NReturnStatement : public NStatement {
public:
    std::shared_ptr<NExpression> expression;
    NReturnStatement(std::shared_ptr<NExpression> expression = nullptr) : expression(expression) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NReturnStatement"; }
};

class NFunctionCall : public NExpression {
public:
    std::shared_ptr<NIdentifier> id;
    std::vector<std::shared_ptr<NExpression>> arguments;
    NFunctionCall(std::shared_ptr<NIdentifier> id, std::vector<std::shared_ptr<NExpression>> arguments) : id(id), arguments(arguments) {}
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
    virtual std::string getType() override { return "NFunctionCall"; }
};