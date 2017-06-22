//
// Created by Haotian on 17/5/25.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include <vector>
#include <string>
#include "../../include/DataStructure.h"
#include "../BufferManager/BufferManager.h"
#include "../IndexManager/IndexManager.h"

using namespace std;
using namespace MINISQL_BASE;

class RecordManager {
public:
    RecordManager(BufferManager *bm, IndexManager *im) : bm(bm), im(im) {}
    ~RecordManager() = default;
    bool createTable(string &table);
    bool dropTable(string &table);

    bool createIndex(string &table, string &index);
    bool dropIndex(string &table, string &index);

    bool insertRecord(Table &table, Tuple &record);
    bool selectRecord(Table &table, vector<string> &attr, vector<Cond> &cond);
    bool selectRecord(Table &table, vector<string> &attr, vector<Cond> &cond, IndexHint &indexHint);

    bool deleteRecord(Table &table, vector<Cond> &cond);

private:
    BufferManager *bm;
    IndexManager *im;

    void dumpResult(Result &res) const;
};


#endif //MINISQL_RECORDMANAGER_H
