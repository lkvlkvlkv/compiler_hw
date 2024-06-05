%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
extern int yylineno; // 定義行號變量
extern int yycolumn; // 定義列號變量
extern char* yytext; // 定義文本指針

%}

%locations
%union {
    int intval;
    double doubleval;
    char *strval;
}

%token <intval> INT
%token <doubleval> FLOAT
%token <strval> IDENTIFIER

%token <strval> KW_VOID KW_INT KW_DOUBLE KW_FLOAT KW_CHAR
%token <strval> KW_IF KW_ELSE KW_WHILE KW_FOR KW_DO KW_RETURN
%token <strval> KW_BREAK KW_CONTINUE
%token <strval> KW_INCLUDE HEADER_FILE_END

%token <strval> EQ LE GE NE UNKNOWN

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
    | Statement
    ;

FunctionDefinition:
      type Identifier '(' ParameterDeclarationList ')' ';'
    ;

FunctionDeclaration:
      type Identifier '(' ParameterDeclarationList ')' FunctionBlock
    ;

FunctionBlock:
      '{' FunctionStatements '}'
    ;

FunctionStatements:
      /* empty */
    | FunctionStatements FunctionStatement
    ;

FunctionStatement:
      Statement
    | KW_RETURN Expression ';' {
        printf("KW_RETURN:\n");
      }
    | KW_RETURN ';' {
        printf("KW_RETURN:\n");
      }
    ;

FunctionCallStatement:
      Identifier '(' ParameterList ')' ';'{
        printf("FunctionCallStatement:\n");
      }
    ;

ParameterList:
      /* empty */
    | ParameterList ',' Parameter
    | Parameter
    ;

Parameter:
      Expression
    ;

ParameterDeclarationList:
      /* empty */
    | ParameterDeclarationList ',' ParameterDeclaration
    | ParameterDeclaration
    ;

ParameterDeclaration:
      type IDENTIFIER
    ;

ForBlock:
      ForBlockStatement
    | '{' ForBlockStatements '}'
    ;

ForBlockStatements:
      /* empty */
    | ForBlockStatements ForBlockStatement
    ;

ForBlockStatement:
      Statement
    | KW_CONTINUE ';' {
        printf("KW_CONTINUE:\n");
      }
    | KW_BREAK ';' {
        printf("KW_BREAK:\n");
      }
    ;

Statement:
      AssignmentStatement
    | DeclarationStatement
    | IfStatement
    | WhileStatement
    | ForStatement
    | FunctionCallStatement {
        printf("FunctionCallStatement:\n");
      }
    ;
  
IncludeStatement:
      '#' KW_INCLUDE HEADEREXPRESSION {
        printf("IncludeStatement:\n");
      }
    ;
  
HEADEREXPRESSION:
      '<' Identifier HEADER_FILE_END '>'
      | '"' Identifier HEADER_FILE_END '"'

DeclarationStatement:
      type DeclarationList ';' {
        printf("DeclarationStatement:\n");
      }
    ;

DeclarationList:
      DeclarationList ',' Identifier
    | DeclarationList ',' AssignExpression
    | AssignExpression
    | Identifier
    ;

AssignExpression:
      Identifier '=' Expression {
        printf("AssignExpression:\n");
      }
    ;

AssignmentStatement:
      AssignExpression ';'
    ;

IfStatement:
      KW_IF '(' Condition ')' Block
    | KW_IF '(' Condition ')' Block KW_ELSE Block
    ;

Block:
      Statement
    | BlockWithBrace
    ;

BlockWithBrace:
      '{' Statements '}'
    ;

Statements:
      /* empty */
    | Statements Statement
    ;

WhileStatement:
      KW_WHILE '(' Condition ')' Block
    ;

ForStatement:
      KW_FOR '(' initStatement  Condition ';' AssignExpression ')' ForBlock {
        printf("End ForStatement:\n");
      }
    ;

initStatement:
      DeclarationStatement
    | AssignmentStatement
    ;

Condition:
      Expression '<'  Expression
    | Expression '>'  Expression
    | Expression EQ   Expression
    | Expression LE   Expression
    | Expression GE   Expression
    | Expression NE   Expression
    ;

Expression:
      Term
    | Expression '+' Term
    | Expression '-' Term
    ;

Term:
      Factor
    | Term '*' Factor
    | Term '/' Factor
    ;

Factor:
      Identifier
    | Numeric
    | '(' Expression ')'
    ;

Identifier:
      IDENTIFIER {
        printf("Identifier: %s\n", $1);
      }
    ;

Numeric:
      INT
    | FLOAT
    ;

type:
      KW_INT
    | KW_DOUBLE
    | KW_FLOAT
    | KW_CHAR
    | KW_VOID
    ;

%%  

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d, column %d, near '%s'\n", s, yylloc.first_line, yylloc.first_column, yytext);
    exit(1);
}

int main() {
    return yyparse();
}
