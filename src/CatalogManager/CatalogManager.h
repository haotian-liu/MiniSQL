#ifndef _catalog_H_
#define _catalog_H_

#include <list>

#include "../../include/DataStructure.h"

using namespace MINISQL_BASE;
using namespace std;

class CatalogManager
{
public:
    void create_new_table(const std::string &table_name,
                          int num_of_attrs,
                          std::pair<std::string, SqlValueType> primary_key
    );

    CatalogManager() = default;

    ~CatalogManager() = default;

};

#endif