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

    size_t delete_op(const std::string &table_name, const std::vector<Condition> &condition_list);

    size_t select(const std::string &table_name, const std::vector<Condition> &condition_list);

    size_t select(const std::string &table_name,
                  const std::vector<Condition> &condition_list,
                  const std::vector<std::string> &attr_list
    );

    size_t update(const std::string &table_name,
                  const std::string &attr,
                  const SqlValue &value,
                  const std::vector<Condition> &condition_list
    );

    bool create_table(const std::string &table_name,
                      const std::vector<std::pair<std::string, SqlValueType>> &schema_list
    );

    bool create_index(const std::string &table_name, const std::string &attribute_name);

    bool drop_table(const std::string &table_name);

    bool drop_index(const std::string &table_name, const std::string &attribute_name);

    extern std::string database_name;
    extern bool is_database_assigned;

    std::string get_db_name_prefix();
}


#endif //MINISQL_API_H
