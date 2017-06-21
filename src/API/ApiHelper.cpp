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
        return (rm == nullptr) ? rm = new RecordManager() : rm;
    }

    IndexManager *ApiHelper::getIndexManager()
    {
        return (im == nullptr) ? im = new IndexManager() : im;
    }

    BufferManager *ApiHelper::getBufferManager()
    {
        return (bm == nullptr) ? bm = new BufferManager() : bm;
    }

    ApiHelper *ApiHelper::helper = nullptr;
}