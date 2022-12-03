#ifndef _LEXER_H_
#define _LEXER_H_

#include "token.hpp"

class Lexer {
    std::string src;
    size_t ndx;

    std::string tokBuff;
    char ahead;

public:
    Lexer(std::string& src)
    : src(src)
    , ndx(0)
    {}

    Token nextToken();

private:
    Token lexNumber();
    Token lexIdentifier();

    Token makeToken(Token::Kind kind);

    bool read();
    bool skip();
    bool peek();
};

#endif