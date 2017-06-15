%require "3.0.4"

%{ /* -*- C++ -*- */

/*
 *
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include "parser_public.h"
#include "y.tab.h"

%}

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

    RW_TEST

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
%token <str> T_QSTRING

%type <str> T_ASTRING

%type <dummy> top_stmt
    test
    exit
    dml
    insert

%token
    ddl
    query
    delete_op
    update
    create_table
    create_index
    drop_table
    drop_index

%%

top_input: top_stmt ';' { YYACCEPT; };

top_stmt: exit
    | test
    | dml
    ;

dml:
     insert
    ;

insert: RW_INSERT RW_INTO T_ASTRING RW_VALUES '(' T_ASTRING ')'
    {
        std::cout << $6 << std::endl;
    }
    ;

exit: RW_EXIT { isExit = true; };

test: RW_TEST { std::cerr << "RW_TEST is input\n"; };

T_ASTRING: T_QSTRING | T_STRING;

%%

bool bFlag; /* no meanings. */

