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

    bool createTable(const string &table);

    bool dropTable(const string &table);

    bool createIndex(const Table &table, const SqlValueType &index);

    bool dropIndex(const string &table, const string &index);

    int insertRecord(const Table &table, const Tuple &record);

    bool selectRecord(const Table &table, const vector<string> &attr, const vector<Cond> &cond);

    bool
    selectRecord(const Table &table, const vector<string> &attr, const vector<Cond> &cond, const IndexHint &indexHint);

    bool deleteRecord(const Table &table, const vector<Cond> &cond);

private:
    BufferManager *bm;
    IndexManager *im;

    void dumpResult(const Result &res) const;
};


#endif //MINISQL_RECORDMANAGER_H
