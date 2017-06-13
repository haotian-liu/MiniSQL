//
// Created by yzy on 6/12/17.
//

#include <string>

int get_id(std::string &s)
{
    std::string tok{s};
    int r;

    if (tok == "select")r = RW_SELECT;
    if (tok == "create")r = RW_CREATE;
    if (tok == "drop")r = RW_DROP;
    if (tok == "table")r = RW_TABLE;
    if (tok == "index")r = RW_INDEX;
    if (tok == "from")r = RW_FROM;
    if (tok == "where")r = RW_WHERE;
    if (tok == "insert")r = RW_INSERT;
    if (tok == "delete")r = RW_DELETE;
    if (tok == "update")r = RW_UPDATE;
    if (tok == "and")r = RW_AND;
    if (tok == "into")r = RW_INTO;
    if (tok == "values")r = RW_VALUES;

    return yylval.i = r;
}
