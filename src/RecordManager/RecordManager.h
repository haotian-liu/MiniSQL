//
// Created by Haotian on 17/5/25.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include <vector>
#include <string>
#include "../../include/DataStructure.h"

using namespace std;

class RecordManager {
public:
    RecordManager();
    ~RecordManager();
    bool createTable(string table);
    bool dropTable(string table);

    bool createIndex(string table, string index);
    bool dropIndex(string table, string index);

    bool insertRecord(string table, const char *record, int length);
    bool selectRecord(string table, vector<string> &attr, vector<MINISQL_BASE::Cond> &cond);

    bool deleteRecord(string table, vector<MINISQL_BASE::Cond> &cond);
};


#endif //MINISQL_RECORDMANAGER_H
