/************************* calc.y ***********************/
%{
   #include <stdio.h>
   #include <ctype.h>
   #include "calc.h"
   int yylex (void);
   void yyerror (char const *);
   int debug;
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
  : '\n'       {  }
  | expr '\n'  { printf ("\t%.10g\n", $1); }
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
  extern FILE *yyin;
  char cmd[80]="TYPE ";
  yyin = fopen(argv[1], "r");
  printf("    ��J�����\"%s\"���e�p�U�G\n",argv[1]);
  strcat(cmd, argv[1]);
  system(cmd);
  if (argc ==3 )
    debug = 1;
  else
    debug = 0;
  printf("    �z�Lbison��yyparse()�v�@��R���G�p�U�G\n");
  nodestackTop = NULL;
  yyparse();
  if (debug && feof(yyin))
    printf("%s", nodestackToString());
  fclose(yyin);
  return 0;
}
