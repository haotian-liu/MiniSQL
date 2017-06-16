//
// Created by Haotian on 17/6/10.
//

#ifndef MINISQL_DATASTRUCTURE_H
#define MINISQL_DATASTRUCTURE_H

#include <string>
#include <vector>

// define basic types we support
#define MINISQL_TYPE_INT 0
#define MINISQL_TYPE_CHAR 1
#define MINISQL_TYPE_DATES 2
#define MINISQL_TYPE_FLOAT 3
#define MINISQL_TYPE_NULL 4

struct Element {
    std::string attrName, val;
};

struct Tuple {
    std::vector<Element> element;
};

struct Table {
    Table() {};
    Table(const Table& T) : DbName(T.DbName), Name(T.Name), attrCnt(T.attrCnt), recordLength(T.recordLength), recordCnt(T.recordCnt), size(T.size) {};

    std::string DbName, Name;
    int attrCnt, recordLength, recordCnt, size;
};

struct Row {
    std::vector<std::string> col;
};

struct Result {
    std::vector<Row> row;
};

#endif //MINISQL_DATASTRUCTURE_H
