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
    std::ifstream ifs(meta_file_name);
    std::string tb_name;
    while (!ifs.eof())
    {
        ifs >> tb_name;
    }
}

CatalogManager::~CatalogManager()
{

}

void CatalogManager::Flush() const
{

}
