//
// Created by yzy on 6/8/17.
//

#ifndef MINISQL_INTERPRETER_H
#define MINISQL_INTERPRETER_H

#include "QueryRequest.h"

extern QueryRequest *query;

class Interpreter
{

};

void main_repl_loop();

void dispatch();

void exec_file(const std::string &file_name);

#endif //MINISQL_INTERPRETER_H
