#include <string>
#include <iostream>

#include "src/Interpreter/Interpreter.h"

int main(int argc, char *argv[])
{
    std::string file_name;
    switch (argc)
    {
        case 1:
            main_repl_loop();
            break;
        case 2:
            file_name = argv[1];
            break;
        default:
            return 1;
    }
    return 0;
}
