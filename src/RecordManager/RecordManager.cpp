//
// Created by Haotian on 17/5/25.
//

#include "RecordManager.h"

RecordManager::RecordManager() {

}

RecordManager::~RecordManager() {

}

bool RecordManager::createTable(string table) {
    string tableFile = MINISQL_BASE::tableFile(table);
    FILE *fp = fopen(tableFile, "w+");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

bool RecordManager::dropTable(string table) {
    string tableFile = MINISQL_BASE::tableFile(table);
    bm->removeFileNode(tableFile);
    remove(tableFile);
    return true;
}

bool RecordManager::createIndex(string table, string index) {
    string indexFile = MINISQL_BASE::indexFile(table, index);
    FILE *fp = fopen(indexFile, "w+");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

bool RecordManager::dropIndex(string table, string index) {
    string indexFile = MINISQL_BASE::indexFile(table, index);
    bm->removeFileNode(indexFile);
    remove(indexFile);
    return true;
}

bool RecordManager::insertRecord(MINISQL_BASE::Table table, MINISQL_BASE::Tuple record) {
    char *block = bm->getFileBlock(MINISQL_BASE::tableFile(table.Name));
    int length = table.recordLength + 1;
    int blocks = MINISQL_BASE::BlockSize / length;
    int offset = 1;
    string strFixed;
    int lengthOffset;

    while (true) {
        for (int i=0; i<blocks; i++) {
            if (block[i * length] != MINISQL_BASE::UnUsed) { continue; }
            for (auto attr = record.element.begin(); attr < record.element.end(); attr++) {
                switch (MINISQL_BASE::M(attr->type)) {
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
            block[i * length] = MINISQL_BASE::Used;
            bm->setDirty(block);
            return true;
        }
        cerr << "All blocks are used!" << endl;
        return false;
    }
}

bool RecordManager::selectRecord(string table, vector<string> &attr, vector<MINISQL_BASE::Cond> &cond) {
    return false;
}

bool RecordManager::deleteRecord(string table, vector<MINISQL_BASE::Cond> &cond) {
    return false;
}
