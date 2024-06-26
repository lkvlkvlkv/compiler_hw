/************************* calc.y ***********************/
%{
   #include <stdio.h>
   #include <math.h>
   #include "calc.h"
   int yylex (void);
   void yyerror (char const *);
%}
%union {
  double val;
  struct nodeTag *nodeptr;
  }
%token <val> NUM
%token <nodeptr> VAR FUN
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
  | expr '\n'  { printf ("\t%.10g\n", $1); }
  | error '\n' { yyerrok; }
  ;
expr
  : NUM { $$ = $1; }
  | VAR { $$ = $1->value.var; }
  | VAR '=' expr { $$ = $3; $1->value.var = $3; }
  | FUN '(' expr')' { $$ = (*($1->value.funptr))($3); }
  | expr '+' expr { $$ = $1 + $3; }
  | expr '-' expr { $$ = $1 - $3; }
  | expr '*' expr { $$ = $1 * $3; }
  | expr '/' expr { $$ = $1 / $3; }
  |'-' expr %prec NEG { $$ = -$2; }
  | expr '^' expr { $$ = pow ($1, $3); }
  |'(' expr')' { $$ = $2; }
  ;
%%
void yyerror (char const *s)
{
  printf ("%s\n", s);
}
struct funTag
{
  char const *fname;
  double (*ftype) (double);
} funs[] = { "sin",  sin,
             "cos",  cos,
             "atan", atan,
             "ln",   log,
             "exp",  exp,
             "sqrt", sqrt,
           } ;
void stackfuns(void)
{
  int i;
  struct nodeTag *p;
  for (i=0; i<sizeof(funs)/sizeof(struct funTag); i++)
  {
    p = putsym(funs[i].fname, FUN);
    p->value.funptr = funs[i].ftype;
  }
}
int main(int argc, char *argv[])
{
  printf("    輸入資料檔\"mfcalc.txt\"內容如下：\n");
  system("TYPE calc.txt");
  printf("    透過bison的yyparse()逐一剖析結果如下：\n");
  nodestackTop = NULL;
  stackfuns ();
  return yyparse ();
}
