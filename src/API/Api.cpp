//
// Created by yzy on 6/8/17.
//

#include <string>
#include <stdexcept>

#include "../RecordManager/RecordManager.h"
#include "Api.h"
#include "ApiHelper.h"

namespace Api
{
    bool use_database(const std::string &db_name)
    {
        Api::is_database_assigned = true;
        Api::database_name = db_name;
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

    bool create_table(const std::string &table_name,
                      const std::vector<std::pair<std::string, SqlValueType>> &schema_list)
    {
        return false;
    }

    bool create_index(const std::string &table_name, const std::string &attribute_name)
    {
        return false;
    }

    bool drop_table(const std::string &table_name)
    {
        return false;
    }

    bool drop_index(const std::string &table_name, const std::string &attribute_name)
    {
        return false;
    }

    size_t select(const std::string &table_name, const std::vector<Condition> &condition_list)
    {
        return 0;
    }

    size_t select(const std::string &table_name, const std::vector<Condition> &condition_list,
                  const std::vector<std::string> &attr_list)
    {
        return 0;
    }
}
