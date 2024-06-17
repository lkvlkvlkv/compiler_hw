%{
#include <string>
#include <iostream>
#include "yacc.tab.h"

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;

#define SAVE_TOKEN yylval.string = new std::string(yytext)
#define TOKEN(t) yylval.token = t; return t

extern int yylineno;
extern char* yytext;
int yycolumn = 1;

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

"int"           { std::cout << "TOKEN: KW_INT\n";      TOKEN(KW_INT); }
"float"         { std::cout << "TOKEN: KW_FLOAT\n";    TOKEN(KW_FLOAT); }
"if"            { std::cout << "TOKEN: KW_IF\n";       TOKEN(KW_IF); }
"else"          { std::cout << "TOKEN: KW_ELSE\n";     TOKEN(KW_ELSE); }
"while"         { std::cout << "TOKEN: KW_WHILE\n";    TOKEN(KW_WHILE); }
"for"           { std::cout << "TOKEN: KW_FOR\n";      TOKEN(KW_FOR); }
"break"         { std::cout << "TOKEN: KW_BREAK\n";    TOKEN(KW_BREAK); }
"continue"      { std::cout << "TOKEN: KW_CONTINUE\n"; TOKEN(KW_CONTINUE); }
"do"            { std::cout << "TOKEN: KW_DO\n";       TOKEN(KW_DO); }
"return"        { std::cout << "TOKEN: KW_RETURN\n";   TOKEN(KW_RETURN); }
"include"       { std::cout << "TOKEN: KW_INCLUDE\n";  TOKEN(KW_INCLUDE); }
"void"          { std::cout << "TOKEN: KW_VOID\n";     TOKEN(KW_VOID); }

[0-9]+                   { SAVE_TOKEN;     std::cout << "TOKEN: INT " << yytext << "\n";          return NUMBER; }
[0-9]+"."[0-9]*          { SAVE_TOKEN;     std::cout << "TOKEN: FLOAT " << yytext << "\n";        return FRAC_NUMBER; }
[A-Za-z_][A-Za-z0-9_]*   { SAVE_TOKEN;     std::cout << "TOKEN: IDENTIFIER " << yytext << "\n";   return IDENTIFIER; }
[A-Za-z_][A-Za-z0-9_]*.h { SAVE_TOKEN;     std::cout << "HEADER FILE " << yytext << "\n";         return HEADER_FILE; }

"=="            { std::cout << "TOKEN: EQ\n";   TOKEN(COM_EQ); }
"<="            { std::cout << "TOKEN: LE\n";   TOKEN(COM_LE); }
">="            { std::cout << "TOKEN: GE\n";   TOKEN(COM_GE); }
"<>"            { std::cout << "TOKEN: NE\n";   TOKEN(COM_NE); }

"<"             { std::cout << "TOKEN: '<'\n";  TOKEN(COM_LT);  }
">"             { std::cout << "TOKEN: '>'\n";  TOKEN(COM_GT);  }

"="             { std::cout << "TOKEN: '='\n";  TOKEN(OP_ASSIGN); }
"+"             { std::cout << "TOKEN: '+'\n";  TOKEN(OP_PLUS);   }
"-"             { std::cout << "TOKEN: '-'\n";  TOKEN(OP_MINUS);  }
"*"             { std::cout << "TOKEN: '*'\n";  TOKEN(OP_MULT);   }
"/"             { std::cout << "TOKEN: '/'\n";  TOKEN(OP_DIV);    }

"("             { std::cout << "TOKEN: '('\n";  TOKEN(LPAREN); }
")"             { std::cout << "TOKEN: ')'\n";  TOKEN(RPAREN); }
"{"             { std::cout << "TOKEN: '{'\n";  TOKEN(LBRACE); }
"}"             { std::cout << "TOKEN: '}'\n";  TOKEN(RBRACE); }

","             { std::cout << "TOKEN: ','\n";  TOKEN(COMMA);    }
";"             { std::cout << "TOKEN: ';'\n";  TOKEN(SEMICOLON); }
"#"             { std::cout << "TOKEN: '#'\n";  TOKEN(HASH);     }
"\""            { std::cout << "TOKEN: '\"'\n"; TOKEN(QUOTE); }

[ \t\r]+        { ; }
\n              { yycolumn = 1; }

.               { std::cout << "unknown token: " << yytext << "\n"; yyterminate(); }

%%
