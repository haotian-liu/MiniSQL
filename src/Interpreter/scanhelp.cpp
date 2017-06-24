//
// Created by yzy on 6/12/17.
//

#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <climits>

///
/// \param s
/// \return T_STRING as default.
int get_id(const std::string &s)
{
    std::string tok{s};
    int r = T_STRING;

    std::transform(tok.begin(), tok.end(), tok.begin(), [](unsigned char c)
    { return std::tolower(c); });

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
    if (tok == "set")r = RW_SET;
    if (tok == "unique")r = RW_UNIQUE;
    if (tok == "primary")r = RW_PRIMARY;
    if (tok == "key")r = RW_KEY;

    if (tok == "use")r = RW_USE;
    if (tok == "database")r = RW_DATABASE;
    if (tok == "execfile")r = RW_EXECFILE;

    if (tok == "int")r = RW_INTEGER;
    if (tok == "char")r = RW_CHAR;
    if (tok == "float")r = RW_FLOAT;

    if (tok == "naive")r = RW_TEST;

    if (tok == "exit")r = RW_EXIT;

    return yylval.i = r;
}
