//
// Created by Haotian on 17/5/25.
//

#include "RecordManager.h"
#include "../../include/DataStructure.h"

RecordManager::RecordManager() {

}

RecordManager::~RecordManager() {

}

bool RecordManager::createTable(string table) {
    string tableFileStr = tableFile(table);
    bm->getFileBlock(tableFileStr);
    return true;
}

bool RecordManager::dropTable(string table) {
    string tableFileStr = tableFile(table);
    bm->removeFileNode(tableFileStr);
    bm->removeFile(tableFileStr);
    return true;
}

bool RecordManager::createIndex(string table, string index) {
    string indexFileStr = indexFile(table, index);
    bm->getFileBlock(indexFileStr);
    return true;
}

bool RecordManager::dropIndex(string table, string index) {
    string indexFileStr = indexFile(table, index);
    bm->removeFileNode(indexFileStr);
    bm->removeFile(indexFileStr);
    return true;
}

bool RecordManager::insertRecord(Table table, Tuple record) {
    char *block = bm->getFileBlock(tableFile(table.Name));
    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    int offset = 1;
    string strFixed;
    int lengthOffset;

    while (true) {
        for (int i=0; i<blocks; i++) {
            if (block[i * length] != UnUsed) { continue; }
            for (auto attr = record.element.begin(); attr < record.element.end(); attr++) {
                switch (attr->type.M()) {
                    case MINISQL_TYPE_CHAR:
                        strFixed = attr->str;
                        lengthOffset = attr->strLength - strFixed.length();
                        if (lengthOffset > 0) {
                            strFixed.insert(0, lengthOffset, 0);
                        }
                        memcpy(block + i * length + offset, strFixed.c_str(), attr->strLength + 1);
                        offset += attr->strLength + 1;
                        break;
                    case MINISQL_TYPE_INT:
                        memcpy(block + i * length + offset, &attr->i, sizeof(int));
                        offset += sizeof(int);
                        break;
                    case MINISQL_TYPE_FLOAT:
                        memcpy(block + i * length + offset, &attr->r, sizeof(float));
                        offset += sizeof(float);
                        break;
                    default:
                        cerr << "Undefined type!!" << endl;
                        break;
                }
            }
            block[i * length] = Used;
            bm->setDirty(block);
            return true;
        }
        cerr << "All blocks are used!" << endl;
        return false;
    }
}

bool RecordManager::selectRecord(Table table, vector<string> &attr, vector<Cond> &cond) {
    char *block = bm->getFileBlock(tableFile(table.Name));
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
        block = bm->getFileBlock(tableFile(table.Name));
    }

    dumpResult(res);
}

bool RecordManager::deleteRecord(string table, vector<Cond> &cond) {
    return false;
}

void RecordManager::dumpResult(Result &res) const {
    for (auto row : res.row) {
        cout << " | ";
        for (auto col : row.col) {
            cout << col << " | ";
        }
        cout << endl;
    }
}
