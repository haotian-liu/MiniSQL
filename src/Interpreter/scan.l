%{ /* -*- C++ -*- */
/*
 * scan.l
 * flex scanner file for SQL interpreter
 *
 * write in C++
 * Author: yzy
 */

#include "parser_public.h"
#include "y.tab.h"

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cassert>

%}

letter      [A-Za-z]
digit       [0-9]
num         {digit}+
s_digit     [+\-]?{digit}
s_num       [+\-]?{num}

%%

[ \t\n]		{}

{s_num}						{ /* std::cout << "T_INT\n"; */ sscanf(yytext, "%d", &yylval.i); return T_INT; }
{s_num}\.{num}				{ /* std::cout << "T_REAL\n"; */ sscanf(yytext, "%lf", &yylval.r); return T_REAL; }
{s_num}\.{num}[Ee]{s_num}	{ sscanf(yytext, "%lf", &yylval.r); return T_REAL; }
\"([^\"\n]|(\"\"))*\"       { yylval.str = std::string(yytext + 1, yyleng - 2); return T_QSTRING; }
`([^\"\n]|(\"\"))*`         { yylval.str = std::string(yytext + 1, yyleng - 2); return T_RQSTRING; }

{letter}({letter}|{digit}|_)*   { return get_id(yylval.str = std::string(yytext)); }

"<"                  {return T_LT;}
"<="                 {return T_LE;}
">"                  {return T_GT;}
">="                 {return T_GE;}
"="                  {return T_EQ;}
"!="                 {return T_NE;}
"<>"                 {return T_NE;}

[*/+\-=<>':;,.|&()]  {return yytext[0];}
<<EOF>>              {return T_EOF;}

.                    { std::cerr << "illegal character [" << yytext[0] << "]\n"; }

%%

#include "scanhelp.cpp"

inline int yywrap()
{
    yy_flush_buffer(YY_CURRENT_BUFFER);
    { BEGIN INITIAL; }
    return 1;
}

