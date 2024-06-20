%{
#include <string>
#include <iostream>
#include <vector>
#include "node.hpp"

#define YYERROR_VERBOSE 1

void yyerror(const char *s);
extern yylex(void);

extern int yylineno;
extern int yycolumn;
extern char* yytext;
NProgram* programBlock;

%}

%locations

%union {
    NProgram *program;
    NType *type;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *identifier;
    std::vector<std::shared_ptr<NStatement>> *stmtvec;
    std::vector<std::shared_ptr<NVariableDeclaration>> *varvec;
    std::vector<std::shared_ptr<NExpression>> *exprvec;
    std::string *string;
    int token;
}

%token <string> NUMBER FRAC_NUMBER IDENTIFIER
%token <string> KW_VOID KW_INT KW_FLOAT
%token <token> KW_IF KW_ELSE KW_WHILE KW_FOR KW_DO KW_RETURN
%token <token> KW_BREAK KW_CONTINUE

%token <token> COM_EQ COM_NE COM_LE COM_GE COM_LT COM_GT
%token <token> OP_ASSIGN OP_PLUS OP_MINUS OP_MULT OP_DIV
%token <token> LPAREN RPAREN LBRACE RBRACE
%token <token> SEMICOLON COMMA

%type <program> Program
%type <stmtvec> GlobalStatements
%type <block> FunctionBlock FunctionStatements Block Statements
%type <stmt> Statement GlobalStatement FunctionDeclaration FunctionDefinition FunctionStatement IfStatement
%type <expr> Condition Expression Term Factor Numeric AssignExpression FunctionCallExpression DeclarationExpression FPDeclaration
%type <varvec> DeclarationList FPDeclarationList
%type <exprvec> FCParameterList
%type <identifier> Identifier
%type <type> type

%start Program

%%

Program:
      GlobalStatements {
        $$ = new NProgram();
        $$->statements = *$1;
        programBlock = $$;
      }
    ;

GlobalStatements:
      GlobalStatement {
        $$ = new std::vector<std::shared_ptr<NStatement>>();
        $$->push_back(std::shared_ptr<NStatement>($1));
      }
    | GlobalStatements GlobalStatement {
        $1->push_back(std::shared_ptr<NStatement>($2));
      } 
    ;

GlobalStatement:
      FunctionDeclaration
    | FunctionDefinition SEMICOLON {
        $$ = $1;
      }
    | DeclarationExpression SEMICOLON {
        $$ = new NExpressionStatement(std::shared_ptr<NExpression>($1));
      }
    ;

FunctionDefinition:
      type Identifier LPAREN FPDeclarationList RPAREN {
        $$ = new NFunctionDefinition(std::shared_ptr<NType>($1), std::shared_ptr<NIdentifier>($2), std::vector<std::shared_ptr<NVariableDeclaration>>(*$4));
      }
    ;

FunctionDeclaration:
      FunctionDefinition FunctionBlock {
        $$ = new NFunctionDeclaration(std::shared_ptr<NFunctionDefinition>(dynamic_cast<NFunctionDefinition*>($1)), std::shared_ptr<NBlock>($2));
      }
    ;

FPDeclarationList:
      /* empty */ {
        $$ = new std::vector<std::shared_ptr<NVariableDeclaration>>();
      }
    | FPDeclarationList COMMA FPDeclaration {
        $1->push_back(std::shared_ptr<NVariableDeclaration>(dynamic_cast<NVariableDeclaration*>($3)));
      }
    | FPDeclaration {
        $$ = new std::vector<std::shared_ptr<NVariableDeclaration>>();
        $$->push_back(std::shared_ptr<NVariableDeclaration>(dynamic_cast<NVariableDeclaration*>($1)));
      }
    ;

FPDeclaration:
      type Identifier {
        $$ = new NVariableDeclaration(std::shared_ptr<NType>($1), std::shared_ptr<NIdentifier>($2), nullptr);
      }
    ;

FunctionBlock:
      LBRACE FunctionStatements RBRACE {
        $$ = $2;
      }
    ;

FunctionStatements:
      FunctionStatement {
        $$ = new NBlock();
        $$->statements.push_back($1);
      }
    | FunctionStatements FunctionStatement {
        $1->statements.push_back($2);
      }
    ;

FunctionStatement:
      Statement
    | KW_RETURN Expression SEMICOLON {
        $$ = new NReturnStatement(std::shared_ptr<NExpression>($2));
      }
    | KW_RETURN SEMICOLON {
        $$ = new NReturnStatement();
      }
    ;

FunctionCallExpression:
      Identifier LPAREN FCParameterList RPAREN {
        std::cout << (*$3).size() << std::endl;
        $$ = new NFunctionCall(std::shared_ptr<NIdentifier>($1), std::vector<std::shared_ptr<NExpression>>(*$3));
      }
    ;

FCParameterList:
      /* empty */ {
        $$ = new std::vector<std::shared_ptr<NExpression>>();
      }
    | FCParameterList COMMA Expression {
        $1->push_back(std::shared_ptr<NExpression>($3));
      }
    | Expression {
        $$ = new std::vector<std::shared_ptr<NExpression>>();
        $$->push_back(std::shared_ptr<NExpression>($1));
      }
    ;


Statement:
      DeclarationExpression SEMICOLON {
        $$ = new NExpressionStatement(std::shared_ptr<NExpression>($1));
      }
    | AssignExpression SEMICOLON {
        $$ = new NExpressionStatement(std::shared_ptr<NExpression>($1));
      }
    | FunctionCallExpression SEMICOLON {
        $$ = new NExpressionStatement(std::shared_ptr<NExpression>($1));
      }
    | IfStatement {
        $$ = $1;
      }
    ;

IfStatement:
      KW_IF LPAREN Condition RPAREN Block {
        $$ = new NIfStatement(std::shared_ptr<NExpression>($3), std::shared_ptr<NBlock>($5));
      }
    | KW_IF LPAREN Condition RPAREN Block KW_ELSE Block {
        $$ = new NIfStatement(std::shared_ptr<NExpression>($3), std::shared_ptr<NBlock>($5), std::shared_ptr<NBlock>($7));
      }
    ;

Block:
      Statement {
        $$ = new NBlock();
        $$->statements.push_back($1);
      }
    | LBRACE Statements RBRACE {
        $$ = $2;
      }
    ;

Statements:
      /* empty */ {
        $$ = new NBlock();
      }
    | Statements Statement {
        $1->statements.push_back($2);
      }
    ;

DeclarationExpression:
      type DeclarationList {
        std::vector<std::shared_ptr<NVariableDeclaration>> *varList = $2;
        for (auto it = varList->begin(); it != varList->end(); it++) {
            (*it)->type = std::shared_ptr<NType>($1);
        }
        $$ = new NVariableDeclarationList(*varList);
      }
    ;

DeclarationList:
      DeclarationList COMMA Identifier {
        $1->push_back(std::make_shared<NVariableDeclaration>(nullptr, std::shared_ptr<NIdentifier>($3), nullptr));
      }
    | DeclarationList COMMA AssignExpression {
        $1->push_back(std::make_shared<NVariableDeclaration>(nullptr, std::shared_ptr<NAssignment>(dynamic_cast<NAssignment*>($3))));
      }
    | AssignExpression {
        $$ = new std::vector<std::shared_ptr<NVariableDeclaration>>();
        $$->push_back(std::make_shared<NVariableDeclaration>(nullptr, std::shared_ptr<NAssignment>(dynamic_cast<NAssignment*>($1))));
      }
    | Identifier {
        $$ = new std::vector<std::shared_ptr<NVariableDeclaration>>();
        $$->push_back(std::make_shared<NVariableDeclaration>(nullptr, std::shared_ptr<NIdentifier>($1), nullptr));
      }
    ;

AssignExpression:
      Identifier OP_ASSIGN Expression {
        $$ = new NAssignment(std::shared_ptr<NIdentifier>($1), std::shared_ptr<NExpression>($3));
      }
    ;

Condition:
      Expression COM_LT Expression {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Expression COM_GT Expression {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Expression COM_EQ Expression {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Expression COM_LE Expression {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Expression COM_GE Expression {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Expression COM_NE Expression {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    ;

Expression:
      Term {
        $$ = $1;
      }
    | Expression OP_PLUS Term {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Expression OP_MINUS Term {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    ;

Term:
      Factor {
        $$ = $1;
      }
    | Term OP_MULT Factor {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    | Term OP_DIV Factor {
        $$ = new NBinaryOperator(std::shared_ptr<NExpression>($1), $2, std::shared_ptr<NExpression>($3));
      }
    ;

Factor:
      Identifier {
        $$ = $1;
      }
    | Numeric {
        $$ = $1;
      }
    | LPAREN Expression RPAREN {
        $$ = $2;
      }
    | FunctionCallExpression {
        $$ = $1;
      }
    ;

Identifier:
      IDENTIFIER {
        $$ = new NIdentifier(*($1));
        delete $1;
      }
    ;

Numeric:
      NUMBER {
        $$ = new NInteger(atol($1->c_str()));
      }
    | FRAC_NUMBER {
        $$ = new NDouble(atof($1->c_str()));
      }
    ;

type:
      KW_INT {
        $$ = new NType(*$1);
      }
    | KW_FLOAT {
        $$ = new NType(*$1);
      }
    | KW_VOID {
        $$ = new NType(*$1);
      }
    ;

%%  

void yyerror(const char *s) {
    std::cout << "Error: " << s << " at line " << yylineno << ", column " << yylloc.first_column << "..." << yylloc.last_column << ", near '" << yytext << "'\n\n";
    return;
}