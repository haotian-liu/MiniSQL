#include <string>
#include <list>
#include <fstream>

#include "CatalogManager.h"

using namespace std;


void CatalogManager::CreateTable(const std::string &table_name, int num_of_attrs,
                                 std::pair<std::string, SqlValueType> primary_key
)
{

}

CatalogManager::CatalogManager()
{
    LoadFromFile();
}

CatalogManager::~CatalogManager()
{
    Flush();
}

void CatalogManager::Flush() const
{

}

void CatalogManager::LoadFromFile()
{
    std::ifstream ifs(meta_file_name);
    std::string tb_name;
    while (!ifs.eof())
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
        } while (!itbfs.eof());
        tb.attrCnt = attr_cnts;
        tables.push_back(tb);
    }
}
