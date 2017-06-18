//
// Created by Haotian on 17/5/25.
//

#include "RecordManager.h"

RecordManager::RecordManager() {

}

RecordManager::~RecordManager() {

}

bool RecordManager::createTable(string table) {
    return false;
}

bool RecordManager::dropTable(string table) {
    return false;
}

bool RecordManager::createIndex(string table, string index) {
    return false;
}

bool RecordManager::dropIndex(string table, string index) {
    return false;
}

bool RecordManager::insertRecord(string table, const char *record, int length) {
    return false;
}

bool RecordManager::selectRecord(string table, vector<string> &attr, vector<MINISQL_BASE::Cond> &cond) {
    return false;
}

bool RecordManager::deleteRecord(string table, vector<MINISQL_BASE::Cond> &cond) {
    return false;
}
