/**************** ���m�O�k�p��� (incalc.y) *************/
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
int yylex (void)
{
  int ch;
  while ((ch=getchar())==' ' || ch=='\t')
    ;                                         /*�����ť�*/
  if (ch=='.' || isdigit(ch))       /*�p���I�Ϊ��ԧB�Ʀr*/
  {
    ungetc(ch, stdin);                /*ch�\�^��J��Ƭy*/
    scanf("%lf", &yylval);  /*�A�q��J��Ƭy�H�B�I��Ū�^*/
    return NUM;                        /*�Ǧ^NUM��ƽs��*/
  }
  if (ch==EOF) return 0;                   /*�ɧ��Ǧ^0��*/
  return ch;            /*�Ǧ^�D�p���I�ΫD���ԧB�Ʀr�r��*/
}
int main (int argc, char *argv[])
{
  printf("    ��J�����\"incalc.txt\"���e�p�U�G\n");
  system("TYPE incalc.txt");
  printf("    �z�Lbison��yyparse()�v�@��R���G�p�U�G\n");
  yyparse();
  return 0;
}
