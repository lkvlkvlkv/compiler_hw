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
};

class NExpression : public Node {
public:
    NExpression(){}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NStatement : public Node {
public:
    NStatement(){}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
	std::shared_ptr<NExpression> expression;

	NExpressionStatement(std::shared_ptr<NExpression> expression)
		: expression(expression) {
	}

	virtual llvm::Value* codeGen(CodeGenContext& context);;
};

class NBlock : public NStatement {
public:
    std::vector<NStatement*> statements;
    NBlock() {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression {
public:
    long long value;
    NDouble(long long value) : value(value) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NIdentifier : public NExpression
{
public:
    std::string name;
    NIdentifier(std::string name) : name(name) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAssignment : public NExpression {
public:
    std::shared_ptr<NIdentifier> lhs;
    std::shared_ptr<NExpression> rhs;
    NAssignment(std::shared_ptr<NIdentifier> lhs, std::shared_ptr<NExpression> rhs) : lhs(lhs), rhs(rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
    std::shared_ptr<NIdentifier> type;
    std::shared_ptr<NIdentifier> id;
    std::shared_ptr<NExpression> assignmentExpr;
    NVariableDeclaration(std::shared_ptr<NIdentifier> type, std::shared_ptr<NIdentifier> id, std::shared_ptr<NExpression> assignmentExpr = nullptr) : type(type), id(id), assignmentExpr(assignmentExpr) {}
    NVariableDeclaration(std::shared_ptr<NIdentifier> type, std::shared_ptr<NAssignment> assignment) : type(type), id(assignment->lhs), assignmentExpr(assignment->rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression {
public:
    int op;
    std::shared_ptr<NExpression> lhs;
    std::shared_ptr<NExpression> rhs;
    NBinaryOperator(std::shared_ptr<NExpression> lhs, int op, std::shared_ptr<NExpression> rhs) : lhs(lhs), op(op), rhs(rhs) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionDeclaration : public NStatement {
public:
    std::shared_ptr<NIdentifier> type;
    std::shared_ptr<NIdentifier> id;
    std::vector<std::shared_ptr<NVariableDeclaration>> arguments;
    NFunctionDeclaration(std::shared_ptr<NIdentifier> type, std::shared_ptr<NIdentifier> id, std::vector<std::shared_ptr<NVariableDeclaration>> arguments) : type(type), id(id), arguments(arguments) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionDefinition : public NStatement {
public:
    std::shared_ptr<NFunctionDeclaration> declaration;
    std::shared_ptr<NBlock> block;
    NFunctionDefinition(std::shared_ptr<NFunctionDeclaration> declaration, std::shared_ptr<NBlock> block) : declaration(declaration), block(block) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NReturnStatement : public NStatement {
public:
    std::shared_ptr<NExpression> expression;
    NReturnStatement(std::shared_ptr<NExpression> expression = nullptr) : expression(expression) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionCall : public NExpression {
public:
    std::shared_ptr<NIdentifier> id;
    std::vector<std::shared_ptr<NExpression>> arguments;
    NFunctionCall(std::shared_ptr<NIdentifier> id, std::vector<std::shared_ptr<NExpression>> arguments) : id(id), arguments(arguments) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
};