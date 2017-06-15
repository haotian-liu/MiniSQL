//
// Created by yzy on 6/13/17.
//

#ifndef MINISQL_QUERYREQUEST_H
#define MINISQL_QUERYREQUEST_H

#include <string>

#include "operator.h"

enum class SqlValueType
{
    Integer,
    String,
    Float
};

struct SqlValue
{
    SqlValueType type;
    int i;
    float r;
    std::string str;
};

struct Condition
{
    std::string name;
    Operator op;
    SqlValue val;
};

enum class QueryType
{
    INSERT,
    DELETE,
    SELECT,
    UPDATE
};

class QueryRequest
{
public:
    QueryType type;

    virtual ~QueryRequest() = 0;
};

#endif //MINISQL_QUERYREQUEST_H
