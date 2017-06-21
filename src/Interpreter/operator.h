//
// Created by yzy on 6/15/17.
//

#ifndef MINISQL_OPERATOR_H
#define MINISQL_OPERATOR_H


enum class Operator
{
    GT_OP,
    GE_OP,
    LT_OP,
    LE_OP,
    EQ_OP,
    NE_OP
};

inline Operator flip_operator(Operator op)
{
    switch (op)
    {
        case Operator::GT_OP:
            return Operator::LT_OP;
        case Operator::GE_OP:
            return Operator::LE_OP;
        case Operator ::LT_OP:
            return Operator ::GT_OP;
        case Operator ::LE_OP:
            return Operator ::GE_OP;
    }
    return op;      // = and != need not flip.
}


#endif //MINISQL_OPERATOR_H
