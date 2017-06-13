%{
/*
 * scan.l
 * flex scanner file for SQL interpreter
 *
 * write in C++
 * Author: yzy
 */

#include "y.tab.h"

#include <cstring>
#include <string>
#include <cstdio>

%}

letter      [A-Za-z]
digit       [0-9]
num         {digit}+
s_digit     [+\-]?{digit}
s_num       [+\-]?{num}

%%

[ \t\n]		{}

{s_num}						{ sscanf(yytext, "%d", yylval.i); return T_INT; }
{s_num}\.{num}				{ sscanf(yytext, "%f", yylval.r); return T_REAL; }
{s_num}\.{num}[Ee]{s_num}	{ sscanf(yytext, "%f", yylval.r); return T_REAL; }
\"([^\"\n]|(\"\"))*\"       { yylval.str = std::string(yytext); return T_STRING; }
\"([^\"\n]|(\"\"))*\n       { std::cerr << "newline in string literal\n"; }

%%

#include "scanhelp.cpp"
