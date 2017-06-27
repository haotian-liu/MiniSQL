//
// Created by yzy on 6/12/17.
//

#ifndef MINISQL_PARSER_PUBLIC_H
#define MINISQL_PARSER_PUBLIC_H

#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <utility>

#include "operator.h"
#include "QueryRequest.h"

#define INPUT_LENGTH 409600

// generated functions
int yylex();

int yyparse();

extern char input_s[INPUT_LENGTH];
extern size_t input_len;

extern std::string file_name;

extern bool isExit;

#ifdef YYSTYPE
#undef YYSTYPE
#endif

#ifdef YY_INPUT
#error YY_INPUT defined
#endif

#define YY_INPUT(i_s, i_len, I_LEN) do { std::strcpy(i_s, input_s); i_len = input_len; } while(0);
// let us assume the buffer should be read-only.

int get_id(const std::string &s);

typedef struct yystype
{
    int i;
    double r;
    std::string str;
    bool b;
    void *dummy;    //for non-final nodes dummy part in AST.
    Operator op;
    SqlValue val;
    Condition cond;
    std::pair<std::string, SqlValueType> schema;
    std::vector<std::string> str_list;
    std::vector<Condition> cond_list;
    std::vector<SqlValue> val_list;
    std::vector<std::pair<std::string, SqlValueType>> schema_list;
    SqlValueType val_type;
} YYSTYPE;

int yyerror(const char *s);

extern "C" int yywrap();

#endif //MINISQL_PARSER_PUBLIC_H
