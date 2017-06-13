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

exit: RW_EXIT { /************/ }

%%

bool bFlag; /* no meanings. */

