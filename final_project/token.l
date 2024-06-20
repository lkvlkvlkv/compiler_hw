%{
#include <string>
#include <iostream>
#include "node.hpp"
#include "grammar.hpp"

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
%option outfile="token.cpp"

%%

\/\/            { BEGIN COMMENT; }
<COMMENT>[^\n]* { ; }
<COMMENT>\n     { BEGIN 0; }

\/\*            { BEGIN MULTI_COMMENT; }
<MULTI_COMMENT>[^*]* { ; }
<MULTI_COMMENT>\*+[^*/]* { ; }
<MULTI_COMMENT>\*+\/ { BEGIN 0; }

"if"            { std::cout << "TOKEN: KW_IF\n";       TOKEN(KW_IF); }
"else"          { std::cout << "TOKEN: KW_ELSE\n";     TOKEN(KW_ELSE); }
"while"         { std::cout << "TOKEN: KW_WHILE\n";    TOKEN(KW_WHILE); }
"for"           { std::cout << "TOKEN: KW_FOR\n";      TOKEN(KW_FOR); }
"break"         { std::cout << "TOKEN: KW_BREAK\n";    TOKEN(KW_BREAK); }
"continue"      { std::cout << "TOKEN: KW_CONTINUE\n"; TOKEN(KW_CONTINUE); }
"do"            { std::cout << "TOKEN: KW_DO\n";       TOKEN(KW_DO); }
"return"        { std::cout << "TOKEN: KW_RETURN\n";   TOKEN(KW_RETURN); }

"int"           { SAVE_TOKEN; std::cout << "TOKEN: KW_INT\n";      return KW_INT;   }
"float"         { SAVE_TOKEN; std::cout << "TOKEN: KW_FLOAT\n";    return KW_FLOAT; }
"void"          { SAVE_TOKEN; std::cout << "TOKEN: KW_VOID\n";     return KW_VOID;  }

[0-9]+                   { SAVE_TOKEN;     std::cout << "TOKEN: INT " << yytext << "\n";          return NUMBER; }
[0-9]+"."[0-9]*          { SAVE_TOKEN;     std::cout << "TOKEN: FLOAT " << yytext << "\n";        return FRAC_NUMBER; }
[A-Za-z_][A-Za-z0-9_]*   { SAVE_TOKEN;     std::cout << "TOKEN: IDENTIFIER " << yytext << "\n";   return IDENTIFIER; }

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

[ \t\r]+        { ; }
\n              { yycolumn = 1; }

.               { std::cout << "unknown token: " << yytext << "\n"; yyterminate(); }

%%
