//
// Created by yzy on 6/8/17.
//

#include <cstdlib>
#include <chrono>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <algorithm>

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

char input_tmp[INPUT_LENGTH];

char half[INPUT_LENGTH];
char *half_ptr;

std::string file_name;

bool isExit = false;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void main_repl_loop [[noreturn]]()
{
    half_ptr = &half[0];
    while (true)
    {
        memset(input_tmp, 0, INPUT_LENGTH);
#ifdef NO_GNU_READLINE
        std::cout << "\n>>> ";
        std::cin.getline(input_tmp, INPUT_LENGTH);
#else
        char *rl = readline("MiniSQL>");
        if (!rl)
            continue;
        std::strcpy(input_tmp, rl);
        std::free(rl);
#endif

        //copy temp input to buffer to form a multi-line completed statement.
        input_tmp[strlen(input_tmp)] = ' ';
        auto tmp_len = strlen(input_tmp);
        bool complete{false};
        for (int i = 0; i < tmp_len; ++i)
        {
            *half_ptr++ = input_tmp[i];
            if (half_ptr[-1] == ';')
            {
                complete = true;
                memset(input_s, 0, INPUT_LENGTH);
                std::strncpy(input_s, half, half_ptr - half);

                half_ptr = half;
                memset(half, 0, INPUT_LENGTH);

                input_len = std::strlen(input_s);
                do_parse();
            }
        }
        if (!complete)
        {
            std::cout << "Statement not complete>" << std::endl;
        }
    }
}

#pragma clang diagnostic pop

///before do_parse() invoke, we should set input_s and input_len to a correct value.
void do_parse()
{
    yyparse();
    if (isExit)
    {
        std::cout << "Bye!\n";
        std::exit(0);
    }
    if (query == nullptr)
    {
        return;
    } else
    {
        auto start_time = std::chrono::system_clock::now();
        dispatch();
        auto finish_time = std::chrono::system_clock::now();
    }
}

void dispatch()
{
    auto e_file = dynamic_cast<ExecFileQuery *>(query);
    if (e_file)
    {
        exec_file(file_name);
        delete e_file;
        query = nullptr;
        return;
    }
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
                                   create_table_query->table_schema_list,
                                   create_table_query->primary_key_name);

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
        auto r = Api::drop_index(Api::get_db_name_prefix() + drop_index_query->index_name);

        delete drop_index_query;
        query = nullptr;
        return;
    }
    return;
}

void exec_file(const std::string &file_name)
{
    std::cout << "Executing SQL file: " << file_name << std::endl;
    std::ifstream file(file_name);

    while (!file.eof())
    {
        memset(input_tmp, 0, INPUT_LENGTH);
        file.getline(input_tmp, INPUT_LENGTH);

        std::cout << input_tmp << std::endl;

        //code below copied from main loop:
        //copy temp input to buffer to form a multi-line completed statement.
        input_tmp[strlen(input_tmp)] = ' ';
        auto tmp_len = strlen(input_tmp);
        bool complete{false};
        for (int i = 0; i < tmp_len; ++i)
        {
            *half_ptr++ = input_tmp[i];
            if (half_ptr[-1] == ';')
            {
                complete = true;
                memset(input_s, 0, INPUT_LENGTH);
                std::strncpy(input_s, half, half_ptr - half);

                half_ptr = half;
                memset(half, 0, INPUT_LENGTH);

                input_len = std::strlen(input_s);
                do_parse();
            }
        }
        if (!complete)
        {
            std::cout << "Statement not complete>" << std::endl;
        }
    }
}
