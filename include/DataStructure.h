//
// Created by Haotian on 17/6/10.
//

#ifndef MINISQL_DATASTRUCTURE_H
#define MINISQL_DATASTRUCTURE_H

#include <string>
#include <iostream>
#include <vector>

// define basic types we support
#define MINISQL_TYPE_INT 0
#define MINISQL_TYPE_CHAR 1
#define MINISQL_TYPE_DATES 2
#define MINISQL_TYPE_FLOAT 3
#define MINISQL_TYPE_NULL 4

// define basic cond we support

#define MINISQL_COND_EQUAL 0
#define MINISQL_COND_UEQUAL 1
#define MINISQL_COND_LEQUAL 2
#define MINISQL_COND_GEQUAL 3
#define MINISQL_COND_LESS 4
#define MINISQL_COND_MORE 5

namespace MINISQL_BASE {
    struct Cond {
        Cond() = default;
        Cond(std::string attr, std::string value, int cond) : attr(attr), value(value), cond(cond) {}
        int cond;
        std::string attr, value;
    };
    inline int getSize(int type, int size=0) {
        switch (type) {
            case MINISQL_TYPE_INT:
                return sizeof(int);
            case MINISQL_TYPE_FLOAT:
                return sizeof(float);
            case MINISQL_TYPE_CHAR:
                return size;
            default:
                std::cerr << "Undefined type!!!" << std::endl;
        }
    }

    inline std::string dbFile(std::string db) { return db + ".db"; }
    inline std::string tableFile(std::string table) { return table + ".tb"; }
    inline std::string indexFile(std::string table, std::string index) { return table + "_" + index + ".ind"; }

    inline int getDegree(int type) {
        int blockSize = 24; // fixme !!!!! add BM Controller!!!
        int keySize = getSize(type);
        int degree = blockSize / (keySize + sizeof(int));

        return degree;
    }

    enum class SqlValueType {
        Integer,
        String,
        Float
    };

    inline int M(SqlValueType& tp) {
        switch (tp) {
            case SqlValueType::Integer:
                return MINISQL_TYPE_INT;
            case SqlValueType::Float:
                return MINISQL_TYPE_FLOAT;
            case SqlValueType::String:
                return MINISQL_TYPE_CHAR;
        }
    }

    struct SqlValue {
        SqlValueType type;
        int i;
        float r;
        std::string str;
    };

    typedef struct SqlValue Element;

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
}
#endif //MINISQL_DATASTRUCTURE_H
