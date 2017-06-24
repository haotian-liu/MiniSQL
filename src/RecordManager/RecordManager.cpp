//
// Created by Haotian on 17/5/25.
//

#include "RecordManager.h"
#include "../../include/DataStructure.h"

bool RecordManager::createTable(const string &table) {
    string tableFileStr = tableFile(table);
    bm->create_table(tableFileStr);
    return true;
}

bool RecordManager::dropTable(const string &table) {
    string tableFileStr = tableFile(table);
    bm->removeFile(tableFileStr);
    return true;
}

bool RecordManager::createIndex(const string &table, const string &index) {
    string indexFileStr = indexFile(table, index);
    bm->create_table(indexFileStr);
    return true;
}

bool RecordManager::dropIndex(const string &table, const string &index) {
    string indexFileStr = indexFile(table, index);
    bm->removeFile(indexFileStr);
    return true;
}

bool RecordManager::insertRecord(const Table &table, const Tuple &record) {
    string tableName = tableFile(table.Name);
    unsigned int blockID = bm->get_last_block(tableName);
    char *block = bm->get_block(tableName, blockID);
    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    int offset = 1;
    string strFixed;
    int lengthOffset;

    bool validBlock = false;

    for (int i=0; i<blocks; i++) {
        if (block[i * length] != UnUsed) { continue; }
        validBlock = true;
        block += i * length;
        break;
    }

    if (!validBlock) {
        block = bm->get_block(tableName, ++blockID, true); // get next block (should be empty) and get the first unit
    }

    for (auto attr = record.element.begin(); attr < record.element.end(); attr++) {
        switch (attr->type.M()) {
            case MINISQL_TYPE_CHAR:
                strFixed = attr->str;
                lengthOffset = attr->strLength - strFixed.length();
                if (lengthOffset > 0) {
                    strFixed.insert(0, lengthOffset, 0);
                }
                memcpy(block + offset, strFixed.c_str(), attr->strLength + 1);
                offset += attr->strLength + 1;
                break;
            case MINISQL_TYPE_INT:
                memcpy(block + offset, &attr->i, sizeof(int));
                offset += sizeof(int);
                break;
            case MINISQL_TYPE_FLOAT:
                memcpy(block + offset, &attr->r, sizeof(float));
                offset += sizeof(float);
                break;
            default:
                cerr << "Undefined type!!" << endl;
                break;
        }
    }
    block[0] = Used;
    bm->setDirty(blockID);
    return true;
}

bool RecordManager::selectRecord(const Table &table, const vector<string> &attr, const vector<Cond> &cond) {
    unsigned int blockID = 0;
    char *block = bm->get_block(tableFile(table.Name), blockID);
    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    Tuple tup;
    Row row;
    Result res;

    while (block) {
        for (int i=0; i<blocks; i++) {
            if (block[i * length] != Used) { continue; }
            convertToTuple(block, i * length, table.attrType, tup);
            if (condsTest(cond, tup, table.attrNames)) {
                row = tup.fetchRow(table.attrNames, attr);
                res.row.push_back(row);
            }
        }
        blockID++;
        block = bm->get_block(tableFile(table.Name), blockID);
    }

    dumpResult(res);
}

bool RecordManager::selectRecord(const Table &table, const vector<string> &attr, const vector<Cond> &cond, const IndexHint &indexHint) {
    string tableFileName = tableFile(table.Name);
    int offset;
    if (indexHint.cond.cond == MINISQL_COND_LESS || indexHint.cond.cond == MINISQL_COND_LEQUAL) {
        offset = im->searchHead(tableFileName, indexHint.attrType);
    } else {
        offset = im->search(tableFileName, indexHint.cond.value);
    }

    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    unsigned int blockOffset = offset / blocks;
    char *block = bm->get_block(tableFileName, blockOffset) + offset % BlockSize;
    Tuple tup;
    Row row;
    Result res;
    Element e;
    bool degrade = false;
    int threshold = indexHint.capacity / blocks / 3;
    int cnt = 0;

    while (block) {
        convertToTuple(block, 0, table.attrType, tup);
        if (condsTest(cond, tup, table.attrNames)) {
            row = tup.fetchRow(table.attrNames, attr);
            res.row.push_back(row);
        } else {
            e = tup.fetchElement(table.attrNames, indexHint.attrName);
            if (!indexHint.cond.test(e)) {
                break;
            }
        }
        block = bm->get_block(tableFile(table.Name), blockOffset);
        cnt++;
        if (cnt > threshold) {
            degrade = true;
            break;
        }
    }

    if (!degrade) {
        dumpResult(res);
    } else {
        selectRecord(table, attr, cond);
    }
}

bool RecordManager::deleteRecord(const Table &table, const vector<Cond> &cond) {
    unsigned int blockOffset = 0;
    char *block = bm->get_block(tableFile(table.Name), blockOffset);
    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    Tuple tup;
    Row row;
    Result res;

    while (block) {
        for (int i=0; i<blocks; i++) {
            if (block[i * length] != Used) { continue; }
            convertToTuple(block, i * length, table.attrType, tup);
            if (condsTest(cond, tup, table.attrNames)) {
                block[i * length] = UnUsed;
            }
        }
        bm->setDirty(blockOffset);
        blockOffset++;
        block = bm->get_block(tableFile(table.Name), blockOffset);
    }
}

void RecordManager::dumpResult(const Result &res) const {
    for (auto row : res.row) {
        cout << " | ";
        for (auto col : row.col) {
            cout << col << " | ";
        }
        cout << endl;
    }
}
