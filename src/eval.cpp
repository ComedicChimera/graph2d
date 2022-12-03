#include "ast.hpp"

double ASTBinary::evaluate(double x, double y) {
    auto lhsRes = lhs->evaluate(x, y);
    auto rhsRes = rhs->evaluate(x, y);

    switch (op) {
    case OpKind::ADD:
        return lhsRes + rhsRes;
    case OpKind::SUB:
        return lhsRes - rhsRes;
    case OpKind::MUL:
        return lhsRes * rhsRes;
    case OpKind::DIV:
        return lhsRes / rhsRes;
    case OpKind::POW:
        return pow(lhsRes, rhsRes);
    case OpKind::LOG:
        return log(lhsRes) / log(rhsRes);
    case OpKind::NRT:
        return pow(lhsRes, 1/rhsRes);
    default: // MOD
        return fmod(lhsRes, rhsRes);
    }
}

double ASTUnary::evaluate(double x, double y) {
    auto operandRes = operand->evaluate(x, y);

    switch (op) {
    case OpKind::NEG:
        return -operandRes;
    case OpKind::SIN:
        return sin(operandRes);
    case OpKind::COS:
        return cos(operandRes);
    case OpKind::TAN:
        return tan(operandRes);
    case OpKind::ABS:
        return fabs(operandRes);
    case OpKind::ACOS:
        return acos(operandRes);
    case OpKind::ASIN:
        return asin(operandRes);
    case OpKind::ATAN:
        return atan(operandRes);
    case OpKind::CBRT:
        return cbrt(operandRes);
    case OpKind::CEIL:
        return ceil(operandRes);
    case OpKind::FACT:
        return tgamma(operandRes + 1);
    case OpKind::FLOOR:
        return floor(operandRes);
    case OpKind::SQRT:
        return sqrt(operandRes);
    case OpKind::LOG2:
        return log2(operandRes);
    case OpKind::LOG10:
        return log10(operandRes);
    case OpKind::LN:
        return log(operandRes);
    default: // EXP
        return exp(operandRes);
    }
}