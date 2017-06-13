//
// Created by yzy on 6/12/17.
//

#ifndef MINISQL_PARSER_PUBLIC_H
#define MINISQL_PARSER_PUBLIC_H

#include <string>
#include <iostream>
#include <vector>

int yylex();
int yyparse();

#ifdef YYSTYPE
#undef YYSTYPE
#endif

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
