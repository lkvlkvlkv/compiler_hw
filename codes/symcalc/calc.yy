/************************* calc.y ***********************/
%{
   #include <stdio.h>
   #include <ctype.h>
   #include "calc.h"
   int yylex (void);
   void yyerror (char const *);
   extern char * nodestackToString(); /*�w�q��calc.l�ɮ�*/
%}
%union {
  double val;
  struct nodeTag *nodeptr;
  }
%token <val> NUM
%token <nodeptr> VAR
%type <val> expr
%right '='
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'
%%
input
  : /*empty*/
  | input line
  ;
line
  : '\n'       { printf("%s",nodestackToString()); }
  | expr '\n'   { printf ("\t%.10g\n", $1); }
  | error '\n' { yyerrok; }
  ;
expr
  : NUM { $$ = $1; }
  | VAR { $$ = $1->value.var; }
  | VAR '=' expr { $$ = $3; $1->value.var = $3; }
  | expr '+' expr { $$ = $1 + $3; }
  | expr '-' expr { $$ = $1 - $3; }
  | expr '*' expr { $$ = $1 * $3; }
  | expr '/' expr { $$ = $1 / $3; }
  |'-' expr %prec NEG { $$ = -$2; }
  | expr '^' expr { $$ = pow ($1, $3); }
  |'(' expr ')'  { $$ = $2; }
  ;
%%
void yyerror (char const *s)
{
  printf ("%s\n", s);
}
int main(int argc, char *argv[])
{
  printf("    ��J�����\"calc.txt\"���e�p�U�G\n");
  system("TYPE calc.txt");
  printf("    �z�Lbison��yyparse()�v�@��R���G�p�U�G\n");
  return yyparse ();
}
