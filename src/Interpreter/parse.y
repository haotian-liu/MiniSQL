%require "3.0.4"

%{ /* -*- C++ -*- */

/*
 *
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include "QueryRequest.h"
#include "Interpreter.h"
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

    RW_USE
    RW_DATABASE

    RW_INTEGER
    RW_FLOAT
    RW_CHAR

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
%token <str> T_RQSTRING

%type <str> T_ASTRING
%type <str> T_NSTRING

%type <op> operator

%type <val> value

%type <cond> condition

%type <str_list> attr_list;

%type <cond_list> condition_list;
%type <cond_list> op_where;

%type <val_list> value_list;

%type <schema> schema_item;
%type <schema_list> table_schema_definition;

%type <val_type> value_type;

%type <dummy> top_stmt
    test
    exit
    dml
    ddl
    use_database

%type <dummy>
    insert
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
    | ddl
    | use_database
    ;

dml: insert
    | query
    | delete_op
    | update
    ;

ddl: create_table
    | create_index
    | drop_table
    | drop_index
    ;

use_database: RW_USE RW_DATABASE T_NSTRING
    {
        auto use_db_query = new UseDatabaseQuery();
        use_db_query->database_name = $3;

        query = use_db_query;
    }
    ;

create_table: RW_CREATE RW_TABLE T_STRING '(' table_schema_definition ')'
    {
        auto create_table_query = new CreateTableQuery();
        create_table_query->table_name = $3;
        create_table_query->table_schema_list = $5;

        query = create_table_query;
    }
    ;

create_index: RW_CREATE RW_INDEX T_STRING '(' T_STRING ')'
    {
        auto create_index_query = new CreateIndexQuery();
        create_index_query->table_name = $3;
        create_index_query->attr_name = $5;

        query = create_index_query;
    }
    ;

drop_table: RW_DROP RW_TABLE T_STRING
    {
        auto drop_table_query = new DropTableQuery();
        drop_table_query->table_name = $3;

        query = drop_table_query;
    }
    ;

drop_index: RW_DROP RW_INDEX T_STRING '(' T_STRING ')'
    {
        auto drop_index_query = new DropIndexQuery();
        drop_index_query->table_name = $3;
        drop_index_query->attr_name = $5;

        query = drop_index_query;
    }
    ;
update: RW_UPDATE T_NSTRING RW_SET T_NSTRING T_EQ value op_where
    {
        auto update_query = new UpdateQuery();
        update_query->table_name = $2;
        update_query->condition_list = $7;
        update_query->attr = $4;
        update_query->value = $6;

        query = update_query;
    }
    ;

delete_op: RW_DELETE RW_FROM T_NSTRING op_where
    {
        auto delete_query = new DeleteQuery();
        delete_query->table_name = $3;
        delete_query->condition_list = $4;

        query = delete_query;
    }
    ;

query: RW_SELECT attr_list RW_FROM T_NSTRING op_where
    {
        auto select_query = new SelectQuery();
        select_query->table_name = $4;
        select_query->attr_list = $2;
        select_query->condition_list = $5;

        query = select_query;
    }
    | RW_SELECT '*' RW_FROM T_NSTRING op_where
    {
        auto select_query = new SelectQuery();
        select_query->table_name = $4;
        select_query->condition_list = $5;

        select_query->isSelectAll = true;

        query = select_query;
    }
    ;

insert: RW_INSERT RW_INTO T_NSTRING RW_VALUES '(' value_list ')'
    {
        auto insert_query = new InsertQuery();
        insert_query->table_name = $3;
        insert_query->value_list = $6;

        query = insert_query;
    }
    ;

op_where: RW_WHERE condition_list
    {
        $$ = $2;
    }
    | nothing
    {
        $$ = std::vector<Condition>();
    }
    ;

table_schema_definition: table_schema_definition ',' schema_item
    {
        $$ = $1;
        $$.push_back($3);
    }
    | schema_item
    {
        $$ = std::vector<std::pair<std::string, SqlValueType>>();
        $$.push_back($1);
    }
    ;

schema_item: T_STRING value_type
    {
        $$ = std::make_pair($1, $2);
    }
    ;

value_type: RW_INTEGER { $$.type = SqlValueTypeBase::Integer; }
    | RW_FLOAT { $$.type = SqlValueTypeBase::Float; }
    | RW_CHAR '(' T_INT ')' { $$.type = SqlValueTypeBase::String; $$.charSize = $3; }
    ;

attr_list: attr_list ',' T_NSTRING
    {
        $1.push_back($3);
        $$ = $1;
    }
    | T_NSTRING
    {
        $$ = std::vector<std::string>();
        $$.push_back($1);
    }
    ;

condition_list: condition_list RW_AND condition
    {
        $1.push_back($3);
        $$ = $1;
    }
    | condition
    {
        $$ = std::vector<Condition>();
        $$.push_back($1);
    }
    ;

condition: T_NSTRING operator value
    {
        $$.name = $1;
        $$.op = $2;
        $$.val = $3;
    }
    ;

value_list: value_list ',' value
    {
        $1.push_back($3);
        $$ = $1;
    }
    | value
    {
        $$ = std::vector<SqlValue>();
        $$.push_back($1);
    }
    ;

value:
    T_INT { $$.type.type = SqlValueTypeBase::Integer; $$.i = $1; }
    | T_REAL { $$.type.type = SqlValueTypeBase::Float; $$.r = $1; }
    | T_ASTRING { $$.type.type = SqlValueTypeBase::String; $$.str = $1; }
    ;

operator:
    T_LT { $$ = Operator::LT_OP; }
    | T_LE { $$ = Operator::LE_OP; }
    | T_GT { $$ = Operator::GT_OP; }
    | T_GE { $$ = Operator::GE_OP; }
    | T_EQ { $$ = Operator::EQ_OP; }
    | T_NE { $$ = Operator::NE_OP; }
    ;

T_ASTRING: T_QSTRING | T_STRING;
T_NSTRING: T_RQSTRING | T_STRING;

exit: RW_EXIT { isExit = true; };

nothing:
    ;

test: RW_TEST { std::cout << "YOUR INPUT IS TOOOOOO SIMPLE, SOMETIMES NAIVE!\n"; };

%%

bool bFlag; /* no meanings. */

