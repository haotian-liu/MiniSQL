//
// Created by yzy on 6/13/17.
//

#ifndef MINISQL_QUERYREQUEST_H
#define MINISQL_QUERYREQUEST_H

#include <string>

enum class SqlValueType
{
    Integer,
    String,
    Float
};

struct SqlValue
{
    SqlValueType type;
    union
    {
        int i;
        float r;
        std::string str;
    };
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
