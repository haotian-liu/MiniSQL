//
// Created by Haotian on 17/5/25.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include <vector>
#include <string>
#include "../../include/DataStructure.h"
#include "../BufferManager/BufferManager.h"

using namespace std;

struct

class RecordManager {
public:
    RecordManager();
    ~RecordManager();
    bool createTable(string table);
    bool dropTable(string table);

    bool createIndex(string table, string index);
    bool dropIndex(string table, string index);

    bool insertRecord(MINISQL_BASE::Table table, MINISQL_BASE::Tuple record);
    bool selectRecord(string table, vector<string> &attr, vector<MINISQL_BASE::Cond> &cond);

    bool deleteRecord(string table, vector<MINISQL_BASE::Cond> &cond);

private:
    BufferManager *bm;
};


#endif //MINISQL_RECORDMANAGER_H
