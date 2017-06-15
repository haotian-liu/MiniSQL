//
// Created by yzy on 6/13/17.
//

#ifndef MINISQL_QUERYREQUEST_H
#define MINISQL_QUERYREQUEST_H

#include <string>
#include <vector>

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
protected:
    QueryType type;

public:
    QueryType getQueryType() const
    {
        return this->type;
    }

    virtual ~QueryRequest();
};

class SelectQuery final : public QueryRequest
{
public:
    SelectQuery()
            : isSelectAll(false)
    {
        type = QueryType::SELECT;
    }

    ~SelectQuery() override
    {}

    bool isSelectAll;
    std::string table_name;
    std::vector<std::string> attr_list;
    std::vector<Condition> condition_list;
};

class InsertQuery final : public QueryRequest
{
public:
    InsertQuery()
    {
        type = QueryType::INSERT;
    }

    ~InsertQuery() override
    {}

    std::string table_name;
    std::vector<SqlValue> value_list;
};

class UpdateQuery final : public QueryRequest
{
public:
    UpdateQuery()
    {
        type = QueryType::UPDATE;
    }
    std::string table_name;
    std::string attr;
    SqlValue value;
    std::vector<Condition> condition_list;
};

class DeleteQuery final : public QueryRequest
{
public:
    DeleteQuery()
    {
        type = QueryType::DELETE;
    }
    std::string table_name;
    std::vector<Condition> condition_list;
};

#endif //MINISQL_QUERYREQUEST_H
