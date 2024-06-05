%{
#include "yacc.tab.h" // Bison 生成的頭文件，用於 token 定義
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 添加這行以解決 strdup 警告
extern int yylineno; // 添加這行以確保 yylineno 被定義
%}

%x COMMENT
%x MULTI_COMMENT

%option noyywrap
%option yylineno

%%

\/\/            { BEGIN COMMENT; }
<COMMENT>[^\n]* { ; }
<COMMENT>\n     { BEGIN 0; }

\/\*            { BEGIN MULTI_COMMENT; }
<MULTI_COMMENT>[^*]* { ; }
<MULTI_COMMENT>\*+[^*/]* { ; }
<MULTI_COMMENT>\*+\/ { BEGIN 0; }

"int"           { printf("TOKEN: KW_INT\n"); return KW_INT; }
"float"         { printf("TOKEN: KW_FLOAT\n"); return KW_FLOAT; }
"if"            { printf("TOKEN: KW_IF\n"); return KW_IF; }
"else"          { printf("TOKEN: KW_ELSE\n"); return KW_ELSE; }
"while"         { printf("TOKEN: KW_WHILE\n"); return KW_WHILE; }
"for"           { printf("TOKEN: KW_FOR\n"); return KW_FOR; }
"break"         { printf("TOKEN: KW_BREAK\n"); return KW_BREAK; }
"continue"      { printf("TOKEN: KW_CONTINUE\n"); return KW_CONTINUE; }
"do"            { printf("TOKEN: KW_DO\n"); return KW_DO; }
"return"        { printf("TOKEN: KW_RETURN\n"); return KW_RETURN; }
"include"       { printf("TOKEN: KW_INCLUDE\n"); return KW_INCLUDE; }
"void"          { printf("TOKEN: KW_VOID\n"); return KW_VOID; }

[0-9]+          { yylval.intval = atoi(yytext); printf("TOKEN: INT %s\n", yytext); return INT; }
[0-9]+"."[0-9]* { yylval.doubleval = atof(yytext); printf("TOKEN: FLOAT %s\n", yytext); return FLOAT; }
[A-Za-z_][A-Za-z0-9_]* { yylval.strval = strdup(yytext); printf("TOKEN: IDENTIFIER %s\n", yytext); return IDENTIFIER; }

".h"           { printf("TOKEN: HEADER_FILE_END\n"); return HEADER_FILE_END; }

"="             { printf("TOKEN: '='\n"); return '='; }
"=="            { printf("TOKEN: EQ\n"); return EQ; }
"<="            { printf("TOKEN: LE\n"); return LE; }
">="            { printf("TOKEN: GE\n"); return GE; }
"<>"            { printf("TOKEN: NE\n"); return NE; }
"<"             { printf("TOKEN: '<'\n"); return '<'; }
">"             { printf("TOKEN: '>'\n"); return '>'; }
"+"             { printf("TOKEN: '+'\n"); return '+'; }
"-"             { printf("TOKEN: '-'\n"); return '-'; }
"*"             { printf("TOKEN: '*'\n"); return '*'; }
"/"             { printf("TOKEN: '/'\n"); return '/'; }
"("             { printf("TOKEN: '('\n"); return '('; }
")"             { printf("TOKEN: ')'\n"); return ')'; }
"{"             { printf("TOKEN: '{'\n"); return '{'; }
"}"             { printf("TOKEN: '}'\n"); return '}'; }
","             { printf("TOKEN: ','\n"); return ','; }
";"             { printf("TOKEN: ';'\n"); return ';'; }
"#"             { printf("TOKEN: '#'\n"); return '#'; }
"."             { printf("TOKEN: '.'\n"); return '.'; }
"\""             { printf("TOKEN: '\"'\n"); return '"'; }

[ \t\n\r]+      { ; }

.               { fprintf(stderr, "unknown token: %s\n", yytext); }

%%
