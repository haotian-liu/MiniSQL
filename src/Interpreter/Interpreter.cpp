//
// Created by yzy on 6/8/17.
//

#include <string>
#include <iostream>
#include <cstring>

#include "Interpreter.h"

#include "parser_public.h"

char input_s[INPUT_LENGTH];
size_t input_len;

bool isExit = false;

void main_repl_loop()
{
    while (true)
    {
        std::cout << "\n>>> ";
        std::cin.getline(input_s, INPUT_LENGTH);
        input_len = std::strlen(input_s);
        yyparse();
        if (isExit)
        {
            break;
        }
    }
}
