//
// Created by yzy on 6/21/17.
//

#ifndef MINISQL_APIHELPER_H
#define MINISQL_APIHELPER_H

#include <string>
#include "../RecordManager/RecordManager.h"
#include "../IndexManager/IndexManager.h"
#include "../CatalogManager/CatalogManager.h"

namespace Api
{
    extern std::string database_name;
    extern bool is_database_assigned;

    class ApiHelper
    {
    public:
        static ApiHelper *getApiHelper()
        {
            return (helper == nullptr) ? helper = new ApiHelper() : helper;
        }

        RecordManager *getRecordManager();

        IndexManager *getIndexManager();

        BufferManager *getBufferManager();

        CatalogManager *getCatalogManager();

    private:
        ApiHelper() = default;

        static ApiHelper *helper;

        RecordManager *rm = nullptr;
        IndexManager *im = nullptr;
        BufferManager *bm = nullptr;
        CatalogManager *cm = nullptr;
    };
}


#endif //MINISQL_APIHELPER_H
