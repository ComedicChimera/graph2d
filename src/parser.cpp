#include "parser.hpp"

#include <stdexcept>
#include <format>
#include <unordered_map>
#include <numbers>

std::unique_ptr<ASTNode> Parser::parse() {
    next();

    auto lhs = parseExpr();

    want(Token::Kind::EQUALS);

    auto rhs = parseExpr();

    want(Token::Kind::END_OF_LINE);

    return std::make_unique<ASTBinary>(ASTBinary::OpKind::SUB, lhs, rhs);
}

std::unique_ptr<ASTNode> Parser::parseExpr() {
    auto root = parseTerm();

    while (true) {
        ASTBinary::OpKind op;
        if (has(Token::Kind::PLUS))
            op = ASTBinary::OpKind::ADD;
        else if (has(Token::Kind::MINUS))
            op = ASTBinary::OpKind::SUB;
        else
            return root;

        next();

        auto lhs = std::move(root);
        auto rhs = parseTerm();

        root = std::make_unique<ASTBinary>(op, lhs, rhs);
    }
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto root = parseFactor();

    while (true) {
        ASTBinary::OpKind op;
        switch (tok.kind) {
        case Token::Kind::TIMES:
            op = ASTBinary::OpKind::MUL;
            break;
        case Token::Kind::DIV:
            op = ASTBinary::OpKind::DIV;
            break;
        case Token::Kind::MOD:
            op = ASTBinary::OpKind::MOD;
            break;
        default:
            return root;
        }

        next();

        auto lhs = std::move(root);
        auto rhs = parseFactor();

        root = std::make_unique<ASTBinary>(op, lhs, rhs);
    }
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if (has(Token::Kind::MINUS)) {
        next();
        return std::make_unique<ASTUnary>(ASTUnary::OpKind::NEG, parseFactorial());
    } 
    
    return parseFactorial();
}

std::unique_ptr<ASTNode> Parser::parseFactorial() {
    auto powExpr = parsePowExpr();

    if (has(Token::Kind::BANG)) {
        next();
        return std::make_unique<ASTUnary>(ASTUnary::OpKind::FACT, powExpr);
    }

    return powExpr;
}

std::unique_ptr<ASTNode> Parser::parsePowExpr() {
    auto root = parseAtom();
    
    if (has(Token::Kind::POW)) {
        next();

        auto exponent = parsePowExpr();
        return std::make_unique<ASTBinary>(ASTBinary::OpKind::POW, root, exponent);
    }

    return root;
}

static std::unordered_map<std::string, ASTUnary::OpKind> unaryFuncNames {
    { "sin", ASTUnary::OpKind::SIN },
    { "cos", ASTUnary::OpKind::COS },
    { "tan", ASTUnary::OpKind::TAN },
    { "exp", ASTUnary::OpKind::EXP },
    { "floor", ASTUnary::OpKind::FLOOR },
    { "ceil", ASTUnary::OpKind::CEIL },
    { "sqrt", ASTUnary::OpKind::SQRT },
    { "cbrt", ASTUnary::OpKind::CBRT },
    { "ln", ASTUnary::OpKind::LN },
    { "log2", ASTUnary::OpKind::LOG2 },
    { "log10", ASTUnary::OpKind::LOG10 },
    { "asin", ASTUnary::OpKind::ASIN },
    { "acos", ASTUnary::OpKind::ACOS },
    { "atan", ASTUnary::OpKind::ATAN },
    { "abs", ASTUnary::OpKind::ABS },
    { "fact", ASTUnary::OpKind::FACT },
};

static std::unordered_map<std::string, ASTBinary::OpKind> binaryFuncNames {
    { "log", ASTBinary::OpKind::LOG },
    { "nrt", ASTBinary::OpKind::NRT },  
};

static std::unordered_map<std::string, double> constNames {
    { "pi", std::numbers::pi },
    { "e", std::numbers::e },
    { "tau", 2 * std::numbers::pi },
    { "phi", std::numbers::phi }
};

std::unique_ptr<ASTNode> Parser::parseAtom() {
    switch (tok.kind) {
    case Token::Kind::LPAREN:
    {
        next();

        auto expr = parseExpr();

        want(Token::Kind::RPAREN);

        return expr;
    }
    case Token::Kind::PIPE:
    {
        next();

        auto expr = parseExpr();

        want(Token::Kind::PIPE);

        return std::make_unique<ASTUnary>(ASTUnary::OpKind::ABS, expr);
    }   
    case Token::Kind::NUMBER:
    {
        auto num = std::make_unique<ASTNumber>(std::stod(tok.value));
        next();
        return num;
    }
    case Token::Kind::IDENTIFIER:
    {
        if (tok.value == "x") {
            next();
            return std::make_unique<ASTVariable>(true);
        } else if (tok.value == "y") {
            next();
            return std::make_unique<ASTVariable>(false);
        }
        
        auto ufPair = unaryFuncNames.find(tok.value);
        if (ufPair != unaryFuncNames.end()) {
            next();

            want(Token::Kind::LPAREN);

            auto arg = parseExpr();

            want(Token::Kind::RPAREN);

            return std::make_unique<ASTUnary>(ufPair->second, arg);
        }
        
        auto bfPair = binaryFuncNames.find(tok.value);
        if (bfPair != binaryFuncNames.end()) {
            next();

            want(Token::Kind::LPAREN);

            auto firstArg = parseExpr();

            want(Token::Kind::COMMA);

            auto secondArg = parseExpr();

            want(Token::Kind::RPAREN);

            return std::make_unique<ASTBinary>(bfPair->second, firstArg, secondArg);
        }

        auto constValPair = constNames.find(tok.value);
        if (constValPair != constNames.end()) {
            next();

            return std::make_unique<ASTNumber>(constValPair->second);
        }

        throw std::logic_error(std::format("undefined name: {}", tok.value));
    }
    default:
        reject();
    }
}

/* -------------------------------------------------------------------------- */

void Parser::want(Token::Kind kind) {
    if (has(kind))
        next();
    else
        reject();
}

bool Parser::has(Token::Kind kind) {
    return tok.kind == kind;
}

void Parser::next() {
    tok = lexer.nextToken();
}

void Parser::reject() {
    if (tok.kind == Token::Kind::END_OF_LINE)
        throw std::logic_error("unexpected end of line");

    throw std::logic_error(std::format("unexpected token: {}", tok.value));
}