//
// Created by yzy on 6/8/17.
//

#include <string>
#include <stdexcept>
#include <algorithm>

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
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();

        if (!cm->TableExist(table_name))
        {
            std::cout << "Table not found!" << std::endl;
            return 0;
        }

        auto &tb = cm->GetTable(table_name);
        if (tb.attrNames.size() != value_list.size())
        {
            std::cout << "Number of values not fit!" << std::endl;
            return 0;
        }

        size_t i;
        for (i = 0; i < value_list.size(); ++i)
        {
            if (value_list[i].type.type != tb.attrType[i].type)
            {
                std::cout << "Type mismatch!" << std::endl;
                return 0;
            }
            if (value_list[i].type.type == SqlValueTypeBase::String)
            {
                if (value_list[i].str.length() > tb.attrType[i].charSize)
                {
                    std::cout << "String too long!" << std::endl;
                    return 0;
                }
            }
        }

        Tuple t;
        t.element = value_list;
        auto offset = rm->insertRecord(tb, t);

        for (const auto &id: tb.index)
        {
            auto it = std::find(tb.attrNames.begin(), tb.attrNames.end(), id.first);
            im->insert(indexFile(tb.Name, id.first),
                       value_list[it - tb.attrNames.begin()], offset);
        }

        std::cout << "Insert finished. 1 row affected." << std::endl;
        return 1;
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
            std::cout << "Table already exists!" << std::endl;
            return false;
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
        }
        cm->CreateTable(table_name, schema_list, primary_key_name);
        std::cout << "Create table " << table_name << " success." << std::endl;
        cm->Flush();

        return rm->createTable(table_name);
    }

    bool create_index(const std::string &table_name, const std::string &attribute_name, const std::string &index_name)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();

        if (cm->CheckIndexNameExists(index_name))
        {
            std::cout << "Index name exists!" << std::endl;
            return false;
        }

        if (!cm->TableExist(table_name))
        {
            std::cout << "Table not found!" << std::endl;
            return false;
        }
        auto &tb = cm->GetTable(table_name);

        for (auto &i: tb.index)
        {
            if (i.first == attribute_name)
            {
                std::cout << "Index on the attribute exists!" << std::endl;
                return false;
            }
        }

        SqlValueType type;
        size_t i{0};
        for (; i < tb.attrNames.size(); ++i)
        {
            if (tb.attrNames[i] == attribute_name)
            {
                type = tb.attrType[i];
                break;
            }
        }

        auto b1 = rm->createIndex(tb, type);
        auto b2 = im->create(indexFile(table_name, attribute_name), type);
        if (b1 && b2)
        {
            std::cout << "Create index success" << std::endl;
            return true;
        } else
        {
            return false;
        }
    }

    bool drop_table(const std::string &table_name)
    {
        //FIXME
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();
        return rm->dropTable(table_name);
    }

    bool drop_index(const std::string &index_name)
    {
        //FIXME
        std::string table_name;
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();
        return rm->dropIndex(table_name, index_name);
    }

    bool select(const std::string &table_name, const std::vector<Condition> &condition_list)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();

        if (!cm->TableExist(table_name))
        {
            std::cout << "Table not found!" << std::endl;
            return false;
        }

        auto &tb = cm->GetTable(table_name);

        for (const auto &cond: condition_list)
        {
            auto it = std::find(tb.attrNames.begin(), tb.attrNames.end(), cond.name);
            if (it == tb.attrNames.end())
            {
                std::cout << "Attribute in conditions mismatch!" << std::endl;
                return false;
            }
            auto type = tb.attrType[it - tb.attrNames.begin()];
            if (type.type != cond.val.type.type)
            {
                std::cout << "Type in conditions mismatch!" << std::endl;
                return false;
            }
        }

        return select(table_name, condition_list, tb.attrNames);
    }

    bool select(const std::string &table_name, const std::vector<Condition> &condition_list,
                const std::vector<std::string> &attr_list)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();

        if (!cm->TableExist(table_name))
        {
            std::cout << "Table not found!" << std::endl;
            return false;
        }

        auto &tb = cm->GetTable(table_name);
        auto cond_list = std::vector<Cond>();
        for (const auto &it: condition_list)
        {
            cond_list.push_back(it);
        }

        //check attr_list valid.
        for (auto &at: attr_list)
        {
            if (std::find(tb.attrNames.begin(), tb.attrNames.end(), at) == tb.attrNames.end())
            {
                std::cout << "Attribute mismatch!" << std::endl;
                return false;
            }
        }

        if (tb.index.size() == 0 || cond_list.size() == 0)
        {
            return rm->selectRecord(tb, attr_list, cond_list);
        } else
        {
            for (const auto &ind: tb.index)
            {
                for (const auto &cond: cond_list)
                {
                    if (ind.first == cond.attr)
                    {
                        IndexHint hint;
                        hint.attrName = cond.attr;
                        hint.cond = cond;
                        hint.attrType = cond.value.type.M();
                        return rm->selectRecord(tb, attr_list, cond_list, hint);
                    }
                }
            }
            return rm->selectRecord(tb, attr_list, cond_list);
        }
    }

    bool delete_op(const std::string &table_name, const std::vector<Condition> &condition_list)
    {
        auto rm = ApiHelper::getApiHelper()->getRecordManager();
        auto im = ApiHelper::getApiHelper()->getIndexManager();
        auto cm = ApiHelper::getApiHelper()->getCatalogManager();

        if (!cm->TableExist(table_name))
        {
            std::cout << "Table not found!" << std::endl;
            return false;
        }

        auto &tb = cm->GetTable(table_name);

        for (const auto &cond: condition_list)
        {
            auto it = std::find(tb.attrNames.begin(), tb.attrNames.end(), cond.name);
            if (it == tb.attrNames.end())
            {
                std::cout << "Attribute in conditions mismatch!" << std::endl;
                return false;
            }
            auto type = tb.attrType[it - tb.attrNames.begin()];
            if (type.type != cond.val.type.type)
            {
                std::cout << "Type in conditions mismatch!" << std::endl;
                return false;
            }
        }

        auto cond_list = std::vector<Cond>();
        for (const auto &it: condition_list)
        {
            cond_list.push_back(it);
        }

        auto r = rm->deleteRecord(tb, cond_list);
        if (r)
        {
            std::cout << "Delete success" << std::endl;
        } else
        {
            std::cout << "Delete failed!" << std::endl;
        }
        return r;
    }

    bool update(const std::string &table_name, const std::string &attr, const SqlValue &value,
                const std::vector<Condition> &condition_list)
    {
        std::cout << "Not supported." << std::endl;
        return false;
    }
}
