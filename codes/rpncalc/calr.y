/************************* calr.y ***********************/
%{
    #include <stdio.h>
    #define YYSTYPE double
    int yylex(void);
    void yyerror (char const *);
    extern FILE *yyin;
%}
%token NUM
%%
input
  : input line
  | /*empty*/
  ;
line
  : '\n'
  | expr '\n' { printf ("\t%.10g\n", $1); }
  ;
expr
  : NUM           { $$ = $1; }
  | expr expr '+' { $$ = $1 + $2; }
  | expr expr '-' { $$ = $1 - $2; }
;
%%
void yyerror(char const *s)
{
  fprintf (stderr, "%s\n", s);
}
int main (int argc, char *argv[])
{
  char cmd[80]="TYPE ";
  yyin = fopen(argv[1], "r");
  printf("    ��J�����\"%s\"���e�p�U�G\n",argv[1]);
  strcat(cmd, argv[1]);
  system(cmd);
  printf("    �z�Lbison��yyparse()�v�@��R���G�p�U�G\n");
  yyparse();
  return 0;
}
