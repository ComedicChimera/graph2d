#ifndef _AST_H_
#define _AST_H_

#include <memory>

class ASTNode {
public:
    virtual double evaluate(double x, double y) = 0;
};

/* -------------------------------------------------------------------------- */

class ASTBinary : public ASTNode {
public:
    enum class OpKind {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        POW,
        LOG,
        NRT
    };

    OpKind op;
    std::unique_ptr<ASTNode> lhs, rhs;

    ASTBinary(OpKind op, std::unique_ptr<ASTNode>& lhs, std::unique_ptr<ASTNode>& rhs)
    : op(op)
    , lhs(std::move(lhs))
    , rhs(std::move(rhs))
    {}

    double evaluate(double x, double y) override;
};

class ASTUnary : public ASTNode {
public:
    enum class OpKind {
        NEG,
        SIN,
        COS,
        TAN,
        EXP,
        FLOOR,
        CEIL,
        SQRT,
        CBRT,
        LN,
        LOG2,
        LOG10,
        ASIN,
        ACOS,
        ATAN,
        FACT,
        ABS,
    };

    OpKind op;
    std::unique_ptr<ASTNode> operand;

    ASTUnary(OpKind op, std::unique_ptr<ASTNode>& operand)
    : op(op)
    , operand(std::move(operand))
    {}

    ASTUnary(OpKind op, std::unique_ptr<ASTNode>&& operand)
    : op(op)
    , operand(std::move(operand))
    {}

    double evaluate(double x, double y) override;
};

class ASTNumber : public ASTNode {
    double value;

public:
    ASTNumber(double value)
    : value(value)
    {}

    inline double evaluate(double, double) override { return value; }
};

class ASTVariable : public ASTNode {
    bool isX;

public:
    ASTVariable(bool isX)
    : isX(isX)
    {}

    inline double evaluate(double x, double y) override { return isX ? x : y; }
};

#endif