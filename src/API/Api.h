//
// Created by yzy on 6/8/17.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H

#include <cstddef>

#include "../Interpreter/QueryRequest.h"

namespace Api
{
    size_t insert(const InsertQuery &);

    size_t delete_op(const DeleteQuery &);

    size_t select(const SelectQuery &);

    size_t update(const UpdateQuery &);

    bool create_table(const CreateTableQuery &);

    bool create_index(const CreateIndexQuery &);

    bool drop_table(const DropTableQuery &);

    bool drop_index(const DropIndexQuery &);
}


#endif //MINISQL_API_H
