//
// Created by yzy on 6/8/17.
//

#include <string>
#include <iostream>

#include "Interpreter.h"

#define INPUT_LENGTH 2000

void main_repl_loop()
{
    while (true)
    {
        std::cout << "\n>>> ";
        char input_s[INPUT_LENGTH];
        std::cin.getline(input_s, INPUT_LENGTH);
        std::string input{input_s};

        break;
    }
}
