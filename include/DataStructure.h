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
#include "../src/Interpreter/operator.h"

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

#define INT_LENGTH 4
#define FLOAT_LENGTH 4

namespace MINISQL_BASE {
    const int BlockSize = 4096;
    const int MaxBlocks = 128;
    const char UnUsed = 0;
    const char Used = 1;

    inline std::string dbFile(const std::string &db) { return db + ".db"; }

    inline std::string tableFile(const std::string &table) { return table + ".tb"; }

    inline std::string indexFile(const std::string &table, const std::string &index) {
        return table + "_" + index + ".ind";
    }

    enum class SqlValueTypeBase {
        Integer,
        String,
        Float
    };

    struct SqlValueType {
        std::string attrName;
        SqlValueTypeBase type;

        ///following fileds only for attribute type, not a single sql value type.
        bool primary = false;
        size_t charSize; // charSize does not include the terminating zero of string!
        bool unique = false;

        inline int M() const {
            switch (type) {
                case SqlValueTypeBase::Integer:
                    return MINISQL_TYPE_INT;
                case SqlValueTypeBase::Float:
                    return MINISQL_TYPE_FLOAT;
                case SqlValueTypeBase::String:
                    return MINISQL_TYPE_CHAR;
            }
        }

        inline size_t getSize() const {
            switch (M()) {
                case MINISQL_TYPE_INT:
                    return sizeof(int);
                case MINISQL_TYPE_FLOAT:
                    return sizeof(float);
                case MINISQL_TYPE_CHAR:
                    return charSize + 1;
            }
        }

        inline int getDegree() const {
            size_t keySize = getSize();
            int degree = BlockSize / (keySize + sizeof(int));

            return degree;
        }
    };

    struct SqlValue {
        SqlValueType type;
        int i;
        float r;
        std::string str;

        inline size_t M() const {
            switch (type.type) {
                case SqlValueTypeBase::Integer:
                    return MINISQL_TYPE_INT;
                case SqlValueTypeBase::Float:
                    return MINISQL_TYPE_FLOAT;
                case SqlValueTypeBase::String:
                    return MINISQL_TYPE_CHAR;
            }
        }

        bool operator<(const SqlValue &e) const {
            switch (M()) {
                case MINISQL_TYPE_INT:
                    return i < e.i;
                case MINISQL_TYPE_FLOAT:
                    return r < e.r;
                case MINISQL_TYPE_CHAR:
                    return str < e.str;
                default:
                    throw std::runtime_error("Undefined Type!");
            }
        }

        bool operator==(const SqlValue &e) const {
            switch (M()) {
                case MINISQL_TYPE_INT:
                    return i == e.i;
                case MINISQL_TYPE_FLOAT:
                    return r == e.r;
                case MINISQL_TYPE_CHAR:
                    return str == e.str;
                default:
                    throw std::runtime_error("Undefined Type!");
            }
        }

        bool operator>(const SqlValue &e) const { return !operator<(e); }

        bool operator<=(const SqlValue &e) const { return operator<(e) && operator==(e); }

        bool operator>=(const SqlValue &e) const { return !operator<(e) && operator<(e); }

        void reset() {
            str.clear();
            i = 0;
            r = 0;
        }

        std::string toStr() const {
            switch (M()) {
                case MINISQL_TYPE_INT:
                    return std::to_string(i);
                case MINISQL_TYPE_FLOAT:
                    return std::to_string(r);
                case MINISQL_TYPE_CHAR:
                    return this->str;
            }
        }
    };

    typedef struct SqlValue Element;

    struct Row {
        std::vector<std::string> col;
    };

    struct Result {
        std::vector<Row> row;
    };

    struct Tuple {
        std::vector<Element> element;

        Row fetchRow(const std::vector<std::string> &attrTable, const std::vector<std::string> &attrFetch) const {
            Row row;
            bool attrFound;
            row.col.reserve(attrFetch.size());
            for (auto fetch : attrFetch) {
                attrFound = false;
                for (int i = 0; i < attrTable.size(); i++) {
                    if (fetch == attrTable[i]) {
                        row.col.push_back(element[i].toStr());
                        attrFound = true;
                        break;
                    }
                }
                if (!attrFound) {
                    std::cerr << "Undefined attr in row fetching!!" << std::endl;
                }
            }
            return row;
        }

        const Element &fetchElement(const std::vector<std::string> &attrTable, const std::string &attrFetch) const {
            for (int i = 0; i < attrTable.size(); i++) {
                if (attrFetch == attrTable[i]) {
                    return element[i];
                }
            }
            std::cerr << "Undefined attr in element fetching from tuple!!" << std::endl;
        }
    };

    struct Table {
        Table() {};

/*        Table(const Table &T) : Name(T.Name), attrCnt(T.attrCnt), recordLength(T.recordLength),
                                recordCnt(T.recordCnt), size(T.size), attrType(T.attrType), attrNames(T.attrNames),
                                indexNames(T.indexNames) {};*/

        std::string Name;
        int attrCnt, recordLength, recordCnt, size;

        std::vector<SqlValueType> attrType;
        std::vector<std::string> attrNames;
        /// for index, first stands for attr name, second stands for index name.
        std::vector<std::pair<std::string, std::string>> index;

        friend std::ostream &operator<<(std::ostream &os, const Table &table) {
            os << "Name: " << table.Name << " attrCnt: " << table.attrCnt << " recordLength: " << table.recordLength
               << " recordCnt: " << table.recordCnt << " size: " << table.size
               << " attrNames: " << table.attrNames.size();
            return os;
        }
    };

    struct Condition {
        std::string name;
        Operator op;
        SqlValue val;
    };

    struct Cond {
        Cond() = default;

        Cond(const std::string &attr, const Element &value, int cond) : attr(attr), value(value), cond(cond) {}

        Cond(const Condition &condition)
                : attr(condition.name),
                  value(condition.val) {
            switch (condition.op) {
                case Operator::GE_OP:
                    cond = MINISQL_COND_GEQUAL;
                case Operator::LE_OP:
                    cond = MINISQL_COND_LEQUAL;
                case Operator::GT_OP:
                    cond = MINISQL_COND_MORE;
                case Operator::LT_OP:
                    cond = MINISQL_COND_LESS;
                case Operator::EQ_OP:
                    cond = MINISQL_COND_EQUAL;
                case Operator::NE_OP:
                    cond = MINISQL_COND_UEQUAL;
            }
        }

        int cond;
        std::string attr;
        Element value;

        bool test(const Element &e) const {
            switch (cond) {
                case MINISQL_COND_EQUAL:
                    return e == value;
                case MINISQL_COND_LEQUAL:
                    return e <= value;
                case MINISQL_COND_GEQUAL:
                    return e >= value;
                case MINISQL_COND_LESS:
                    return e < value;
                case MINISQL_COND_MORE:
                    return e > value;
                default:
                    std::cerr << "Undefined condition!" << std::endl;
            }
        }
    };

    inline bool condsTest(const std::vector<Cond> &conds, const Tuple &tup, const std::vector<std::string> &attr) {
        int condPos;
        for (Cond cond : conds) {
            condPos = -1;
            for (int i = 0; i < attr.size(); i++) {
                if (attr[i] == cond.attr) {
                    condPos = i;
                    break;
                }
            }
            if (condPos == -1) {
                std::cerr << "Attr not found in cond test!" << std::endl;
            }
            if (!cond.test(tup.element[condPos])) {
                return false;
            }
        }
        return true;
    }

    inline void
    convertToTuple(const char *blockBuffer, int offset, const std::vector<SqlValueType> &attrType, Tuple &tup) {
        const char *block = blockBuffer + offset + 1; // 1 for meta bit
        Element e;
        tup.element.clear();
        for (int i = 0; i < attrType.size(); i++) {
            e.reset();
            e.type = attrType[i];
            switch (attrType[i].M()) {
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

    struct IndexHint {
        Cond cond;
        std::string attrName;
        int attrType;
    };

}
#endif //MINISQL_DATASTRUCTURE_H
