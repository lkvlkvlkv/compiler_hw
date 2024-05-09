/************* ��m�O�k�p��� (rpncalc.y) ***********/
%{
    #include <stdio.h>
    #include <ctype.h>
    #include <math.h>
    #define YYSTYPE double
    int yylex(void);
    void yyerror(char const *);
%}
%token NUM
%%
input
  : /*empty*/
  | input line
  ;
line
  : '\n'
  | expr '\n' { printf ("\t%.10g\n", $1); }
  ;
expr
  : NUM           { $$ = $1; }
  | expr expr '+' { $$ = $1 + $2; }
  | expr expr '-' { $$ = $1 - $2; }
  | expr expr '*' { $$ = $1 * $2; }
  | expr expr '/' { $$ = $1 / $2; }
  | expr expr '^' { $$ = pow ($1, $2); }
  | expr 'n'      { $$ = -$1; }
;
%%
void yyerror(char const *s)       /*���~�ɩI�s���禡*/
{
  fprintf(stderr, "%s\n", s);
}
int yylex (void)
{
  int ch;
  while ((ch=getchar())==' ' || ch=='\t')
    ;                                     /*�����ť�*/
  if (ch=='.' || isdigit(ch))   /*�p���I�Ϊ��ԧB�Ʀr*/
  {
    ungetc(ch, stdin);            /*ch�\�^��J��Ƭy*/
    scanf("%lf", &yylval);    /*�A�q��J�H�B�I��Ū�^*/
    return NUM;                    /*�Ǧ^NUM��ƽs��*/
  }
  if (ch==EOF) return 0;               /*�ɧ��Ǧ^0��*/
  return ch;            /*�Ǧ^�D�p���I�ΫD���ԧB�Ʀr*/
}
int main (int argc, char *argv[])
{
  printf("  ��J�����\"rpncalc.txt\"���e�p�U�G\n");
  system("TYPE rpncalc.txt");
  printf("  �z�Lbison��yyparse()�v�@��R�p�U�G\n");
  yyparse();
  return 0;
}
