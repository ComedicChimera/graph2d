#ifndef _PARSER_H_
#define _PARSER_H_

#include <memory>

#include "ast.hpp"
#include "lexer.hpp"

class Parser {
    Lexer lexer;
    Token tok;

public:
    Parser(std::string& input)
    : lexer(input)
    {}

    std::unique_ptr<ASTNode> parse();

private:
    std::unique_ptr<ASTNode> parseExpr();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseFactorial();
    std::unique_ptr<ASTNode> parsePowExpr();
    std::unique_ptr<ASTNode> parseAtom();

    void next();
    bool has(Token::Kind kind);
    void want(Token::Kind kind);
    void reject();
};

#endif