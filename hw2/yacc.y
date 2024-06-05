%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYERROR_VERBOSE 1

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

%token <strval> KW_VOID KW_INT KW_FLOAT
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
    | DeclarationStatement
    ;

FunctionDeclaration:
      type Identifier '(' ParameterDeclarationList ')' ';' 
    ;

FunctionDefinition:
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
    | KW_RETURN Expression ';'
    | KW_RETURN ';'
    ;

FunctionCallStatement:
      FunctionCallExpression ';'
    ;

FunctionCallExpression:
      Identifier '(' ParameterList ')'
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
    | KW_CONTINUE ';'
    | KW_BREAK ';'
    | KW_RETURN Expression ';'
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
      KW_DO Block KW_WHILE '(' Condition ')' ';'

IncludeStatement:
      '#' KW_INCLUDE HEADEREXPRESSION
    ;
  
HEADEREXPRESSION:
      '<' Identifier HEADER_FILE_END '>'
      | '"' Identifier HEADER_FILE_END '"'

DeclarationExpression:
      type DeclarationList
    ;

DeclarationStatement:
      DeclarationExpression ';'
    ;

DeclarationList:
      DeclarationList ',' Identifier
    | DeclarationList ',' AssignExpression
    | AssignExpression
    | Identifier
    ;

AssignExpression:
      Identifier '=' Expression
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
      KW_FOR '(' ForInitExpression ';' Condition ';' AssignExpression ')' ForBlock
    ;

ForInitExpression:
      DeclarationExpression
    | AssignExpression
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
    | FunctionCallExpression
    ;

Identifier:
      IDENTIFIER {
        printf("Identifier: %s\n", $1);
      }
    ;

Numeric:
      INT {
        printf("Numeric: %d\n", $1);
      }
    | FLOAT {
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
    fprintf(stderr, "Error: %s at line %d, column %d, near '%s'\n", s, yylloc.first_line, yylloc.first_column, yytext);
    exit(1);
}

int main() {
    return yyparse();
}
