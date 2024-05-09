/************************ calc.y ************************/
%{
    #include <stdio.h>
    #include <ctype.h>
    #include <math.h>
    #define YYSTYPE double
    int yylex (void);
    void yyerror (char const *);
%}
%token NUM
%left '-' '+'
%left '*' '/'
%left NEG         /*negation--unary minus*/
%right '^'        /*exponentiation*/
%%
input
  : input line
  | /*empty*/
  ;
line
  : '\n'
  | expr '\n'          { printf ("\t%.10g\n", $1); }
  ;
expr
  : NUM                { $$ = $1; }
  | expr '+' expr      { $$ = $1 + $3; }
  | expr '-' expr      { $$ = $1 - $3; }
  | expr '*' expr      { $$ = $1 * $3; }
  | expr '/' expr      { $$ = $1 / $3; }
  | '-' expr %prec NEG { $$ = -$2; }
  | expr '^' expr      { $$ = pow ($1, $3); }
  | '(' expr ')'       { $$ = $2; }
  ;
%%
void yyerror(char const *s)
{
  fprintf (stderr, "%s\n", s);
}
int main (int argc, char *argv[])
{
  printf("    ��J�����\"calc.txt\"���e�p�U�G\n");
  system("TYPE calc.txt");
  printf("    �z�Lbison��yyparse()�v�@��R���G�p�U�G\n");
  yyparse();
  return 0;
}
