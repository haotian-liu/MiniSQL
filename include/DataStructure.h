//
// Created by Haotian on 17/6/10.
//

#ifndef MINISQL_DATASTRUCTURE_H
#define MINISQL_DATASTRUCTURE_H

#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>

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
    const int BlockSize = 4096;
    const char UnUsed = 0;
    const char Used = 1;

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

    enum class SqlValueTypeBase {
        Integer,
        String,
        Float
    };

    struct SqlValueType {
        SqlValueTypeBase type;
        int charSize;
    };

    inline int M(SqlValueType& tp) {
        switch (tp.type) {
            case SqlValueTypeBase::Integer:
                return MINISQL_TYPE_INT;
            case SqlValueTypeBase::Float:
                return MINISQL_TYPE_FLOAT;
            case SqlValueTypeBase::String:
                return MINISQL_TYPE_CHAR;
        }
    }

    struct SqlValue {
        SqlValueType type;
        int i;
        float r;
        int strLength;
        std::string str;

        bool operator<(SqlValue& e) {
            switch (M(type)) {
                case MINISQL_TYPE_INT:
                    return i < e.i;
                case MINISQL_TYPE_FLOAT:
                    return r < e.r;
                case MINISQL_TYPE_CHAR:
                    return str < e.str;
                default:
                    throw std::runtime_error("Undefined Type!"):
            }
        }

        bool operator==(SqlValue& e) {
            switch (M(type)) {
                case MINISQL_TYPE_INT:
                    return i == e.i;
                case MINISQL_TYPE_FLOAT:
                    return r == e.r;
                case MINISQL_TYPE_CHAR:
                    return str == e.str;
                default:
                    throw std::runtime_error("Undefined Type!"):
            }
        }

        bool operator>(SqlValue &e) { return !operator<(e); }
        bool operator<=(SqlValue &e) { return operator<(e) && operator==(e); }
        bool operator>=(SqlValue &e) { return !operator<(e) && operator<(e); }

        void reset() {
            str.clear();
            i = 0;
            r = 0;
        }
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
        std::vector<SqlValueType> attrType;
    };

    struct Cond {
        Cond() = default;
        Cond(std::string attr, Element value, int cond) : attr(attr), value(value), cond(cond) {}
        int cond;
        std::string attr;
        Element value;

        bool test(Element &a, Element &b) {
            switch (cond) {
                case MINISQL_COND_EQUAL:
                    return a == b;
                case MINISQL_COND_LEQUAL:
                    return a <= b;
                case MINISQL_COND_GEQUAL:
                    return a >= b;
                case MINISQL_COND_LESS:
                    return a < b;
                case MINISQL_COND_MORE:
                    return a > b;
                default:
                    std::cerr << "Undefined condition!" << std::endl;
            }
        }
    };

    inline void convertToTuple(const char *blockBuffer, int offset, std::vector<SqlValueType> &attrType, Tuple &tup) {
        const char *block = blockBuffer + offset + 1; // 1 for meta bit
        Element e;
        for (int i=0; i<attrType.size(); i++) {
            e.reset();
            e.type = attrType[i];
            switch (M(attrType[i])) {
                case MINISQL_TYPE_INT:
                    memcpy(&e.i, block, sizeof(int));
                    block += sizeof(int);
                    break;
                case MINISQL_TYPE_FLOAT:
                    memcpy(&e.r, block, sizeof(float));
                    block += sizeof(float);
                    break;
                case MINISQL_TYPE_CHAR:
                    e.str = block;
                    block += attrType[i].charSize + 1;
                    break;
            }
            tup.element.push_back(e);
        }
    }

    struct Row {
        std::vector<std::string> col;
    };

    struct Result {
        std::vector<Row> row;
    };
}
#endif //MINISQL_DATASTRUCTURE_H
