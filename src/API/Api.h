//
// Created by yzy on 6/8/17.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H

#include <cstddef>
#include <string>
#include <stdexcept>

#include "../Interpreter/QueryRequest.h"

namespace Api
{
    bool use_database(const std::string &db_name);

    size_t insert(const std::string &table_name, const std::vector<SqlValue> &value_list);

    size_t delete_op(const DeleteQuery &);

    size_t select(const SelectQuery &);

    size_t update(const UpdateQuery &);

    bool create_table(const CreateTableQuery &);

    bool create_index(const CreateIndexQuery &);

    bool drop_table(const DropTableQuery &);

    bool drop_index(const DropIndexQuery &);

    extern std::string database_name;
    extern bool is_database_assigned;

    std::string get_db_name_prefix();
}


#endif //MINISQL_API_H
