%{
#include <string>
#include <iostream>

#define YYERROR_VERBOSE 1

void yyerror(const char *s);
int yylex(void);

extern int yylineno;
extern int yycolumn;
extern char* yytext;

%}

%locations

%union {
    int intval;
    float floatval;
    char *strval;
}

%token <intval>   NUMBER
%token <floatval> FRAC_NUMBER
%token <strval>   IDENTIFIER
%token <strval>   HEADER_FILE

%token <strval> KW_VOID KW_INT KW_FLOAT
%token <strval> KW_IF KW_ELSE KW_WHILE KW_FOR KW_DO KW_RETURN
%token <strval> KW_BREAK KW_CONTINUE
%token <strval> KW_INCLUDE

%token <strval> COM_EQ COM_NE COM_LE COM_GE COM_LT COM_GT
%token <strval> OP_ASSIGN OP_PLUS OP_MINUS OP_MULT OP_DIV
%token <strval> LPAREN RPAREN LBRACE RBRACE
%token <strval> SEMICOLON COMMA
%token <strval> HASH QUOTE

%start Program

%%

Program:
      GlobalStatements
    ;

GlobalStatements:
      /* empty */
    | GlobalStatements GlobalStatement
    ;

GlobalStatement:
      IncludeStatement
    | FunctionDeclaration
    | FunctionDefinition
    | DeclarationStatement
    ;

FunctionDeclaration:
      type Identifier LPAREN ParameterDeclarationList RPAREN SEMICOLON 
    ;

FunctionDefinition:
      type Identifier LPAREN ParameterDeclarationList RPAREN FunctionBlock 
    ;

FunctionBlock:
      LBRACE FunctionStatements RBRACE
    ;

FunctionStatements:
      /* empty */
    | FunctionStatements FunctionStatement
    ;

FunctionStatement:
      Statement
    | KW_RETURN Expression SEMICOLON
    | KW_RETURN SEMICOLON
    ;

FunctionCallStatement:
      FunctionCallExpression SEMICOLON
    ;

FunctionCallExpression:
      Identifier LPAREN ParameterList RPAREN
    ;

ParameterList:
      /* empty */
    | ParameterList COMMA Parameter
    | Parameter
    ;

Parameter:
      Expression
    ;

ParameterDeclarationList:
      /* empty */
    | ParameterDeclarationList COMMA ParameterDeclaration
    | ParameterDeclaration
    ;

ParameterDeclaration:
      type IDENTIFIER
    ;

ForBlock:
      ForBlockStatement
    | LBRACE ForBlockStatements RBRACE
    ;

ForBlockStatements:
      /* empty */
    | ForBlockStatements ForBlockStatement
    ;

ForBlockStatement:
      Statement
    | KW_CONTINUE SEMICOLON
    | KW_BREAK SEMICOLON
    | KW_RETURN Expression SEMICOLON
    ;

Statement:
      AssignmentStatement
    | DeclarationStatement
    | IfStatement
    | WhileStatement
    | ForStatement
    | DoWhileStatement
    | FunctionCallStatement
    ;

DoWhileStatement:
      KW_DO Block KW_WHILE LPAREN Condition RPAREN SEMICOLON

IncludeStatement:
      HASH KW_INCLUDE HEADEREXPRESSION
    ;
  
HEADEREXPRESSION:
      COM_LT HEADER_FILE COM_GT
      | QUOTE HEADER_FILE QUOTE

DeclarationExpression:
      type DeclarationList
    ;

DeclarationStatement:
      DeclarationExpression SEMICOLON
    ;

DeclarationList:
      DeclarationList COMMA Identifier
    | DeclarationList COMMA AssignExpression
    | AssignExpression
    | Identifier
    ;

AssignExpression:
      Identifier OP_ASSIGN Expression
    ;

AssignmentStatement:
      AssignExpression SEMICOLON
    ;

IfStatement:
      KW_IF LPAREN Condition RPAREN Block
    | KW_IF LPAREN Condition RPAREN Block KW_ELSE Block
    ;

Block:
      Statement
    | BlockWithBrace
    ;

BlockWithBrace:
      LBRACE Statements RBRACE
    ;

Statements:
      /* empty */
    | Statements Statement
    ;

WhileStatement:
      KW_WHILE LPAREN Condition RPAREN Block
    ;

ForStatement:
      KW_FOR LPAREN ForInitExpression SEMICOLON Condition SEMICOLON AssignExpression RPAREN ForBlock
    ;

ForInitExpression:
      DeclarationExpression
    | AssignExpression
    ;

Condition:
      Expression COM_LT  Expression
    | Expression COM_GT  Expression
    | Expression COM_EQ   Expression
    | Expression COM_LE   Expression
    | Expression COM_GE   Expression
    | Expression COM_NE   Expression
    ;

Expression:
      Term
    | Expression OP_PLUS Term
    | Expression OP_MINUS Term
    ;

Term:
      Factor
    | Term OP_MULT Factor
    | Term OP_DIV Factor
    ;

Factor:
      Identifier
    | Numeric
    | LPAREN Expression RPAREN
    | FunctionCallExpression
    ;

Identifier:
      IDENTIFIER {
        printf("Identifier: %s\n", $1);
      }
    ;

Numeric:
      NUMBER {
        printf("Numeric: %d\n", $1);
      }
    | FRAC_NUMBER {
        printf("Numeric: %f\n", $1);
      }
    ;

type:
      KW_INT
    | KW_FLOAT
    | KW_VOID
    ;

%%  

void yyerror(const char *s) {
    std::cout << "Error: " << s << " at line " << yylineno << ", column " << yylloc.first_column << "..." << yylloc.last_column << ", near '" << yytext << "'\n\n";
    return;
}

int main() {
    return yyparse();
}
