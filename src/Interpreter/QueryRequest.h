//
// Created by yzy on 6/13/17.
//

#ifndef MINISQL_QUERYREQUEST_H
#define MINISQL_QUERYREQUEST_H

#include <string>

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
