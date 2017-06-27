#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <algorithm>

#include "CatalogManager.h"
#include "../API/ApiHelper.h"

using namespace std;


void CatalogManager::CreateTable(const std::string &table_name,
                                 const std::vector<std::pair<std::string, SqlValueType>> &schema_list,
                                 const std::string &primary_key_name
)
{
    Table tb;
    tb.Name = table_name;
    tb.attrCnt = (int) schema_list.size();
    uint32_t len{0};
    char auto_ind{'A'};

    auto rm = Api::ApiHelper::getApiHelper()->getRecordManager();

    for (const auto &sch: schema_list)
    {
        len += sch.second.getSize();
        tb.attrNames.push_back(sch.first);
        auto t = sch.second;
        t.attrName = sch.first;
        tb.attrType.push_back(t);
        if (sch.first == primary_key_name)
        {
            (tb.attrType.end() - 1)->primary = true;
            (tb.attrType.end() - 1)->unique = true;
        }
/*        if (!(tb.attrType.end() - 1)->primary && (tb.attrType.end() - 1)->unique)
        {
            tb.index.push_back(std::make_pair(*(tb.attrNames.end() - 1), std::string("auto_ind_") + (auto_ind++)));
            rm->createIndex(tb, )
        }*/
    }

    tb.recordLength = len;
    tb.recordCnt = 0;
    for (auto &t: tb.attrType)
    {
        if (t.unique && !t.primary)
        {
            tb.index.push_back(std::make_pair(t.attrName, std::string("auto_ind_") + (auto_ind++)));
            rm->createIndex(tb, t);
        }
    }
    tables.push_back(tb);
}

CatalogManager::CatalogManager()
        : tables(std::vector<Table>())
{
    LoadFromFile();
}

CatalogManager::~CatalogManager()
{
    Flush();
}

void CatalogManager::Flush() const
{
    std::ofstream ofs(meta_file_name);
    ofs << tables.size() << std::endl;

    for (const auto &tb: tables)
    {
        ofs << tb.Name << std::endl;
        ofs << tb.recordCnt << std::endl;
        std::ofstream otbfs(tb.Name + ".catalog");
        uint16_t i{0};

        otbfs << tb.attrNames.size() << std::endl;
        for (const auto &attr_name: tb.attrNames)
        {
            otbfs << attr_name << std::endl;
            const auto &attr = tb.attrType[i];
            switch (attr.type)
            {
                case SqlValueTypeBase::Integer:
                    otbfs << "int" << std::endl;
                    break;
                case SqlValueTypeBase::Float:
                    otbfs << "float" << std::endl;
                    break;
                case SqlValueTypeBase::String:
                    otbfs << "char" << std::endl;
                    break;
            }
            if (attr.type == SqlValueTypeBase::String)
            {
                otbfs << attr.charSize << std::endl;
            } else
            {
                otbfs << 0 << std::endl;
            }
            otbfs << (attr.primary ? 1 : 0) << std::endl;
            otbfs << (attr.unique ? 1 : 0) << std::endl;
            auto ind = std::find_if(tb.index.begin(), tb.index.end(),
                                    [&attr_name](const std::pair<std::string, std::string> &p)
                                    {
                                        return p.first == attr_name;
                                    });
            if (ind != tb.index.end())
            {
                otbfs << 1 << std::endl << ind->second << std::endl;
            } else
            {
                otbfs << 0 << std::endl << "-" << std::endl;
            }
            ++i;
        }
        otbfs.close();
    }
    ofs.close();
}

void CatalogManager::LoadFromFile()
{
    std::ifstream ifs(meta_file_name);
    if (!ifs.is_open())
    {
        std::ofstream touch(meta_file_name);
        return;
    }
    size_t tables_count{0};
    ifs >> tables_count;

    auto rm = Api::ApiHelper::getApiHelper()->getRecordManager();

    std::string tb_name;
    for (auto i = 0; i < tables_count; ++i)
    {
        ifs >> tb_name;
        auto file_name = tb_name + ".catalog";
        std::ifstream itbfs(file_name);

        Table tb;
        std::vector<SqlValueType> ind_vec;
        //size_t len{0};
        ifs >> tb.recordCnt;

        tb.Name = tb_name;

        uint16_t attr_cnts{0};
        uint16_t record_length{0};

        size_t attr_counts{0};
        itbfs >> attr_counts;
        for (auto ci = 0; ci < attr_counts; ++ci)
        {
            std::string attr_name, type_name, index_name;
            uint16_t isPri, isUni, isInd, size;
            SqlValueType type;

            itbfs >> attr_name >> type_name >> size >> isPri >> isUni >> isInd >> index_name;
            ++attr_cnts;

            tb.attrNames.push_back(attr_name);
            if (type_name == "int")
            {
                type.type = SqlValueTypeBase::Integer;
            } else if (type_name == "char")
            {
                type.type = SqlValueTypeBase::String;
                type.charSize = size;
            } else if (type_name == "float")
            {
                type.type = SqlValueTypeBase::Float;
            } else
            {
                valid_assert(false);
            }
            record_length += type.getSize();
            type.primary = isPri != 0;
            type.unique = isUni != 0;
            type.attrName = attr_name;
            tb.attrType.push_back(type);
            if (isInd)
            {
                auto ind = std::make_pair(attr_name, index_name);
                tb.index.push_back(ind);
                ind_vec.push_back(type);
            }
        }
        tb.attrCnt = attr_cnts;
        tb.recordLength = record_length;
        tables.push_back(tb);
        for(auto &it: ind_vec)
        {
            rm->createIndex(tb, it);
        }
    }
}

bool CatalogManager::TableExist(const std::string &table_name) const
{
    return std::find_if(tables.begin(), tables.end(), [&table_name](const Table &tb)
    {
        return (tb.Name == table_name);
    }) != tables.end();
}

Table &CatalogManager::GetTable(const std::string &table_name)
{
    return *std::find_if(tables.begin(), tables.end(), [&table_name](const Table &tb)
    {
        return (tb.Name == table_name);
    });
}

bool CatalogManager::CheckIndexNameExists(const std::string &index_name) const
{
    for (const auto &t: tables)
    {
        for (auto &i: t.index)
        {
            if (i.second == index_name) return true;
        }
    }
    return false;
}

bool CatalogManager::RemoveTable(const Table &table)
{
    if (std::find_if(tables.begin(), tables.end(), [&table](const Table &tb)
    { return tb.Name == table.Name; }) == tables.end())
        return false;
    tables.erase(std::find_if(tables.begin(), tables.end(), [&table](const Table &tb)
    { return tb.Name == table.Name; }));
    return true;
}

Table &CatalogManager::GetTableWithIndex(const std::string &index_name)
{
    for (auto &t: tables)
    {
        for (const auto &i: t.index)
        {
            if (i.second == index_name) return t;
        }
    }
}

