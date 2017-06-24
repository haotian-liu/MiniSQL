//
// Created by yzy on 6/8/17.
//

#include <string>
#include <stdexcept>

#include "../RecordManager/RecordManager.h"
#include "../CatalogManager/CatalogManager.h"
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
            return std::string();
            //throw std::runtime_error("database not assigned!");
        }

        return std::string("db_") + database_name + "_";
    }

    size_t insert(const std::string &table_name, const std::vector<SqlValue> &value_list)
    {
        auto real_table_name = table_name;
        return 0;
    }

    bool create_table(const std::string &table_name,
                      const std::vector<std::pair<std::string, SqlValueType>> &schema_list,
                      const std::string &primary_key_name
    )
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto num_of_attr = schema_list.size();
        for(auto &it: schema_list)
        {
            if(it.first == primary_key_name)
            {

            }
            it.second.type == SqlValueTypeBase ::String;

        }
        if(!primary_key_name.empty())
        {
            auto pp = new Attr();
        }

        return rm->createTable(table_name);
    }

    bool create_index(const std::string &table_name, const std::string &attribute_name)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        return rm->createIndex(table_name, attribute_name);
    }

    bool drop_table(const std::string &table_name)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        return rm->dropTable(table_name);
    }

    bool drop_index(const std::string &table_name, const std::string &attribute_name)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        return rm->dropIndex(table_name, attribute_name);
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

    size_t delete_op(const std::string &table_name, const std::vector<Condition> &condition_list)
    {
        return 0;
    }

    size_t update(const std::string &table_name, const std::string &attr, const SqlValue &value,
                  const std::vector<Condition> &condition_list)
    {
        return 0;
    }
}
