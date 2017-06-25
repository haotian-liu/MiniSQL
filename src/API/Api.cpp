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
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();

        if (cm->TableExist(table_name))
        {
            std::cout << "Table exist" << std::endl;
        }

        auto num_of_attr = schema_list.size();

        for (auto &it: schema_list)
        {
            if (it.second.type == SqlValueTypeBase::String)
            {
                if (it.second.charSize < 1 || it.second.charSize > 255)
                {
                    std::cout << "Char count out of range" << std::endl;
                    return false;
                }
            }
        }

        if (!primary_key_name.empty())
        {
            bool pri_schema_found = false;
            SqlValueType primary_key_type;
            for (auto &it: schema_list)
            {
                if (it.first == primary_key_name)
                {
                    pri_schema_found = true;
                    primary_key_type = it.second;
                }
            }
            if (!pri_schema_found)
            {
                std::cout << "Primary key not found!" << std::endl;
                return false;
            }

            cm->CreateTable(table_name, schema_list, primary_key_name);
            cm->Flush();
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

    bool drop_index(const std::string &index_name)
    {
        std::string table_name;
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        return rm->dropIndex(table_name, index_name);
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
