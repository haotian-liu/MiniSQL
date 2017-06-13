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
    RW_CREATE
    RW_DROP
    RW_TABLE
    RW_INDEX
    RW_SELECT
    RW_FROM
    RW_WHERE
    RW_INSERT
    RW_DELETE
    RW_UPDATE
    RW_AND
    RW_INTO
    RW_VALUES

    RW_SET
    RW_EXIT

    T_LT
    T_GT
    T_GE
    T_LE
    T_EQ
    T_NE

    T_EOF

    NO_TOKEN

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