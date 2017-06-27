#ifndef _catalog_H_
#define _catalog_H_

#include <list>
#include <map>

#include "../../include/DataStructure.h"

using namespace MINISQL_BASE;
using namespace std;

class CatalogManager final
{
public:
    void CreateTable(const std::string &table_name,
                                     const std::vector<std::pair<std::string, SqlValueType>> &schema_list,
                     const std::string &primary_key_name
    );

    bool TableExist(const std::string &table_name) const;

    Table &GetTable(const std::string &table_name);

    bool CheckIndexNameExists(const std::string &index_name) const;

    Table &GetTableWithIndex(const std::string &index_name);

    bool RemoveTable(const Table &table);

    CatalogManager();

    ~CatalogManager();

    void Flush() const;

    bool isValid() const
    { return validFlag; }

    mutable std::map<std::string, int> kv;

private:
    void LoadFromFile();

    bool valid_assert(bool cond)
    {
        return cond ? cond : (validFlag = cond);
    }

private:
    std::vector<Table> tables;

    static constexpr auto meta_file_name = "tables.meta";

    bool validFlag = true;
};
#endif
