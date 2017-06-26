//
// Created by yzy on 6/13/17.
//

#ifndef MINISQL_QUERYREQUEST_H
#define MINISQL_QUERYREQUEST_H

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>
#include <stdexcept>

#include "operator.h"
#include "../../include/DataStructure.h"

using namespace MINISQL_BASE;

enum class QueryType
{
    INSERT,
    DELETE,
    SELECT,
    UPDATE,
    CREATE_TABLE,
    CREATE_INDEX,
    DROP_TABLE,
    DROP_INDEX,
    USE_DATABASE
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

    virtual ~QueryRequest() = 0;
};

class ExecFileQuery final : public QueryRequest
{};

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

class UseDatabaseQuery final : public QueryRequest
{
public:
    UseDatabaseQuery()
    {
        type = QueryType::USE_DATABASE;
    }

    std::string database_name;
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

class CreateTableQuery final : public QueryRequest
{
public:
    CreateTableQuery()
    {
        type = QueryType::CREATE_TABLE;
    }
    std::string table_name;
    std::vector<std::pair<std::string, SqlValueType>> table_schema_list;
    std::string primary_key_name;
};

class CreateIndexQuery final : public QueryRequest
{
public:
    CreateIndexQuery()
    {
        type = QueryType::CREATE_INDEX;
    }
    std::string table_name;
    std::string attr_name;
    std::string index_name;
};

class DropTableQuery final : public QueryRequest
{
public:
    DropTableQuery()
    {
        type = QueryType::DROP_TABLE;
    }
    std::string table_name;
};

class DropIndexQuery final : public QueryRequest
{
public:
    DropIndexQuery()
    {
        type = QueryType::DROP_INDEX;
    }
    std::string index_name;
};
#endif //MINISQL_QUERYREQUEST_H
