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
#include "QueryRequest.h"

#include "parser_public.h"

QueryRequest *query = nullptr;

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
            std::cout << "Bye!\n";
            break;
        }
        if (query == nullptr)
        {
            continue;
        } else
        {
            auto insert_query = dynamic_cast<InsertQuery *>(query);
            if (insert_query)
            {
                delete insert_query;
                query = nullptr;
                continue;
            }

            auto select_query = dynamic_cast<SelectQuery *>(query);
            if (select_query)
            {
                delete select_query;
                query = nullptr;
                continue;
            }

            auto delete_query = dynamic_cast<DeleteQuery *>(query);
            if (delete_query)
            {
                delete delete_query;
                query = nullptr;
                continue;
            }

            auto update_query = dynamic_cast<UpdateQuery *>(query);
            if (update_query)
            {
                delete update_query;
                query = nullptr;
                continue;
            }

            auto create_table_query = dynamic_cast<CreateTableQuery *>(query);
            if (create_table_query)
            {
                delete create_table_query;
                query = nullptr;
                continue;
            }

            auto create_index_query = dynamic_cast<CreateIndexQuery *>(query);
            if (create_index_query)
            {
                delete create_index_query;
                query = nullptr;
                continue;
            }
        }
    }
}
