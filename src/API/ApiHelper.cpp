//
// Created by yzy on 6/21/17.
//
#include <string>

#include "ApiHelper.h"

namespace Api
{
    std::string database_name;
    bool is_database_assigned = false;

    RecordManager *ApiHelper::getRecordManager()
    {
        if (rm == nullptr)
        {
            auto im = getIndexManager();
            auto bm = getBufferManager();
            rm = new RecordManager(bm, im);
        }
        return rm;
    }

    IndexManager *ApiHelper::getIndexManager()
    {
        return (im == nullptr) ? im = new IndexManager() : im;
    }

    BufferManager *ApiHelper::getBufferManager()
    {
        return (bm == nullptr) ? bm = new BufferManager() : bm;
    }

    CatalogManager *ApiHelper::getCatalogManager()
    {
        return (cm == nullptr) ? cm = new CatalogManager() : cm;
    }

    ApiHelper *ApiHelper::helper = nullptr;
}