//
// Created by yzy on 6/8/17.
//

#include <string>
#include <stdexcept>

#include "Api.h"

namespace Api
{
    std::string database_name;
    bool is_database_assigned = false;

    bool use_database(const std::string &db_name)
    {
        is_database_assigned = true;
        database_name = db_name;
    }

    std::string get_db_name_prefix()
    {
        if (!is_database_assigned)
        {
            throw std::runtime_error("database not assigned!");
        }

        return std::string("db_") + database_name + "_";
    }

    size_t insert(const std::string &table_name, const std::vector<SqlValue> &value_list)
    {
        auto real_table_name = Api::get_db_name_prefix() + table_name;
        return 0;
    }
}
