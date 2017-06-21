//
// Created by yzy on 6/21/17.
//

#ifndef MINISQL_APIHELPER_H
#define MINISQL_APIHELPER_H

#include <string>
#include "../RecordManager/RecordManager.h"
#include "../IndexManager/IndexManager.h"

namespace Api
{
    extern std::string database_name;
    extern bool is_database_assigned;

    class ApiHelper
    {
    public:
        RecordManager *getRecordManager();

        IndexManager *getIndexManager();

    private:
        RecordManager *rm;
        IndexManager *im;
    };
}


#endif //MINISQL_APIHELPER_H
