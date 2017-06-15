//
// Created by yzy on 6/8/17.
//

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>

#ifdef NO_GNU_READLINE
#error GNU readline needed
#else

#include <readline/readline.h>

#endif

#include "Interpreter.h"

#include "parser_public.h"

char input_s[INPUT_LENGTH];
size_t input_len;

bool isExit = false;

void main_repl_loop()
{
    while (true)
    {
#ifdef NO_GNU_READLINE
        std::cout << "\n>>> ";
        std::cin.getline(input_s, INPUT_LENGTH);
#else
        char *rl = readline("MiniSQL>");
        if (!rl)
            continue;
        std::strcpy(input_s, rl);
        std::free(rl);
#endif
        input_len = std::strlen(input_s);
        yyparse();
        if (isExit)
        {
            break;
        }
    }
}
