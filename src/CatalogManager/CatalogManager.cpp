#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <algorithm>

#include "CatalogManager.h"

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

    for (const auto &sch: schema_list)
    {
        len += sch.second.getSize();
        tb.attrNames.push_back(sch.first);
        tb.attrType.push_back(sch.second);
        if (sch.first == primary_key_name)
        {
            (tb.attrType.end() - 1)->primary = true;
        }
    }

    tb.recordLength = len;
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

    //debug
    //std::cout << tables.size() << endl;

    for (const auto &tb: tables)
    {
        ofs << tb.Name << std::endl;
        std::ofstream otbfs(tb.Name + ".catalog");
        uint16_t i{0};

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
            otbfs << std::endl;
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

    std::string tb_name;
    while (ifs.peek() != EOF)
    {
        ifs >> tb_name;
        auto file_name = tb_name + ".catalog";
        std::ifstream itbfs(file_name);

        Table tb;
        tb.Name = tb_name;

        uint16_t attr_cnts{0};
        uint16_t record_length{0};
        do
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
            tb.attrType.push_back(type);
            if (isInd)
            {
                auto ind = std::make_pair(attr_name, index_name);
                tb.index.push_back(ind);
            }
        } while (itbfs.peek() != EOF);
        tb.attrCnt = attr_cnts;
        tables.push_back(tb);
    }
}

bool CatalogManager::TableExist(const std::string &table_name) const
{
    return std::find_if(tables.begin(), tables.end(), [&table_name](const Table &tb)
    {
        return (tb.Name == table_name);
    }) != tables.end();
}

