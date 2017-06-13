%{

/*
 *
 */

#include <iostream>
#include <string>

%}

%union
{
    int i;
    double r;
    std::string str;
}

%token
    T_LT
    T_GT
    T_GE
    T_LE
    T_EQ
    T_NE

%token <i> T_INT
%token <r> T_REAL
%token <str> T_STRING

%%

%%

int yyerror(const char *s)
{
    std::cerr << s << std::endl;
}

int yywrap()
{
    return 1;
}