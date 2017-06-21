//
// Created by yzy on 6/8/17.
//

#include <cstdlib>
#include <chrono>
#include <string>
#include <iostream>
#include <cstring>

#ifdef NO_GNU_READLINE
#warning GNU readline is not enabled
#else

#include <readline/readline.h>

#endif

#include "Interpreter.h"
#include "QueryRequest.h"
#include "../API/Api.h"

#include "parser_public.h"
#include "../API/ApiHelper.h"

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
            auto start_time = std::chrono::system_clock::now();
            dispatch();
            auto finish_time = std::chrono::system_clock::now();
        }
    }
}

void dispatch()
{
    auto use_db_query = dynamic_cast<UseDatabaseQuery *>(query);
    if (use_db_query)
    {
        Api::use_database(use_db_query->database_name);
        std::cout << "Database " << use_db_query->database_name << " used." << std::endl;

        delete use_db_query;
        query = nullptr;
        return;
    }
    auto insert_query = dynamic_cast<InsertQuery *>(query);
    if (insert_query)
    {
        auto insert_count = Api::insert(Api::get_db_name_prefix() + insert_query->table_name, insert_query->value_list);

        delete insert_query;
        query = nullptr;
        return;
    }

    auto select_query = dynamic_cast<SelectQuery *>(query);
    if (select_query)
    {
        size_t row_count{0};
        if (select_query->isSelectAll)
        {
            row_count = Api::select(Api::get_db_name_prefix() + select_query->table_name, select_query->condition_list);
        } else
        {
            row_count = Api::select(Api::get_db_name_prefix() + select_query->table_name,
                                    select_query->condition_list,
                                    select_query->attr_list);
        }

        delete select_query;
        query = nullptr;
        return;
    }

    auto delete_query = dynamic_cast<DeleteQuery *>(query);
    if (delete_query)
    {
        delete delete_query;
        query = nullptr;
        return;
    }

    auto update_query = dynamic_cast<UpdateQuery *>(query);
    if (update_query)
    {
        delete update_query;
        query = nullptr;
        return;
    }

    auto create_table_query = dynamic_cast<CreateTableQuery *>(query);
    if (create_table_query)
    {
        auto r = Api::create_table(Api::get_db_name_prefix() + create_table_query->table_name,
                                   create_table_query->table_schema_list);

        delete create_table_query;
        query = nullptr;
        return;
    }

    auto create_index_query = dynamic_cast<CreateIndexQuery *>(query);
    if (create_index_query)
    {
        auto r = Api::create_index(Api::get_db_name_prefix() + create_index_query->table_name,
                                   create_index_query->attr_name);

        delete create_index_query;
        query = nullptr;
        return;
    }

    auto drop_table_query = dynamic_cast<DropTableQuery *>(query);
    if (drop_table_query)
    {
        auto r = Api::drop_table(Api::get_db_name_prefix() + drop_table_query->table_name);

        delete drop_table_query;
        query = nullptr;
        return;
    }

    auto drop_index_query = dynamic_cast<DropIndexQuery *>(query);
    if (drop_index_query)
    {
        auto r = Api::drop_index(Api::get_db_name_prefix() + drop_index_query->table_name, drop_index_query->attr_name);

        delete drop_index_query;
        query = nullptr;
        return;
    }
    return;
}
