#ifndef _catalog_H_
#define _catalog_H_

#include <list>

#include "../../include/DataStructure.h"

using namespace MINISQL_BASE;
using namespace std;

class CatalogManager final
{
public:
    void CreateTable(const std::string &table_name,
                     int num_of_attrs,
                     std::pair<std::string, SqlValueType> primary_key
    );

    CatalogManager();

    ~CatalogManager();

private:
    std::list<Table> tables;

    static constexpr auto meta_file_name = "tables.meta";
};

#endif