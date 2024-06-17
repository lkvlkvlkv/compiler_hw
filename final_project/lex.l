%{
#include <string>
#include <iostream>
#include "yacc.tab.h"

extern int yylineno;
extern char* yytext;
int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng;

#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
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

"int"           { std::cout << "TOKEN: KW_INT\n";      return KW_INT; }
"float"         { std::cout << "TOKEN: KW_FLOAT\n";    return KW_FLOAT; }
"if"            { std::cout << "TOKEN: KW_IF\n";       return KW_IF; }
"else"          { std::cout << "TOKEN: KW_ELSE\n";     return KW_ELSE; }
"while"         { std::cout << "TOKEN: KW_WHILE\n";    return KW_WHILE; }
"for"           { std::cout << "TOKEN: KW_FOR\n";      return KW_FOR; }
"break"         { std::cout << "TOKEN: KW_BREAK\n";    return KW_BREAK; }
"continue"      { std::cout << "TOKEN: KW_CONTINUE\n"; return KW_CONTINUE; }
"do"            { std::cout << "TOKEN: KW_DO\n";       return KW_DO; }
"return"        { std::cout << "TOKEN: KW_RETURN\n";   return KW_RETURN; }
"include"       { std::cout << "TOKEN: KW_INCLUDE\n";  return KW_INCLUDE; }
"void"          { std::cout << "TOKEN: KW_VOID\n";     return KW_VOID; }

[0-9]+                   { yylval.intval = atoi(yytext);     std::cout << "TOKEN: INT " << yytext << "\n";          return NUMBER; }
[0-9]+"."[0-9]*          { yylval.doubleval = atof(yytext);  std::cout << "TOKEN: FLOAT " << yytext << "\n";        return FRAC_NUMBER; }
[A-Za-z_][A-Za-z0-9_]*   { yylval.strval = strdup(yytext);   std::cout << "TOKEN: IDENTIFIER " << yytext << "\n";   return IDENTIFIER; }
[A-Za-z_][A-Za-z0-9_]*.h { yylval.strval = strdup(yytext);   std::cout << "HEADER FILE " << yytext << "\n";        return HEADER_FILE; }

"=="            { std::cout << "TOKEN: EQ\n";   return COM_EQ;  }
"<="            { std::cout << "TOKEN: LE\n";   return COM_LE;  }
">="            { std::cout << "TOKEN: GE\n";   return COM_GE;  }
"<>"            { std::cout << "TOKEN: NE\n";   return COM_NE;  }

"<"             { std::cout << "TOKEN: '<'\n";  return COM_LT;  }
">"             { std::cout << "TOKEN: '>'\n";  return COM_GT;  }

"="             { std::cout << "TOKEN: '='\n";  return OP_ASSIGN; }
"+"             { std::cout << "TOKEN: '+'\n";  return OP_PLUS;   }
"-"             { std::cout << "TOKEN: '-'\n";  return OP_MINUS;  }
"*"             { std::cout << "TOKEN: '*'\n";  return OP_MULT;   }
"/"             { std::cout << "TOKEN: '/'\n";  return OP_DIV;    }

"("             { std::cout << "TOKEN: '('\n";  return LPAREN; }
")"             { std::cout << "TOKEN: ')'\n";  return RPAREN; }
"{"             { std::cout << "TOKEN: '{'\n";  return LBRACE; }
"}"             { std::cout << "TOKEN: '}'\n";  return RBRACE; }

","             { std::cout << "TOKEN: ','\n";  return COMMA;     }
";"             { std::cout << "TOKEN: ';'\n";  return SEMICOLON; }
"#"             { std::cout << "TOKEN: '#'\n";  return HASH;      }
"\""            { std::cout << "TOKEN: '\"'\n"; return QUOTE;     }

[ \t\r]+        { ; }
\n              { yycolumn = 1; }

.               { fprintf(stderr, "unknown token: %s\n", yytext); }

%%
