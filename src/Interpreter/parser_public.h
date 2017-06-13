//
// Created by yzy on 6/12/17.
//

#ifndef MINISQL_PARSER_PUBLIC_H
#define MINISQL_PARSER_PUBLIC_H

#include <string>
#include <iostream>
#include <vector>

#define INPUT_LENGTH 2000

int yylex();
int yyparse();

extern char input_s[INPUT_LENGTH];
extern size_t input_len;

#ifdef YYSTYPE
#undef YYSTYPE
#endif

#ifdef YY_INPUT
#error YY_INPUT defined
#endif

#define YY_INPUT(input_s, input_len, INPUT_LENGTH)
// let us assume the buffer should be read-only.

int get_id(std::string &s);

typedef struct yystype
{
    int i;
    double r;
    std::string str;
    void *dummy;    //for non-final nodes dummy part in AST.
    std::vector<std::string> str_list;
} YYSTYPE;

extern "C" inline int yyerror(const char *s)
{
    std::cerr << s << std::endl;
}

extern "C" inline int yywrap()
{
    return 1;
}

#endif //MINISQL_PARSER_PUBLIC_H
