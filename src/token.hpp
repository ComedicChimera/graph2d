#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

struct Token {
public:
    enum class Kind {
        PLUS,
        MINUS,
        TIMES,
        DIV,
        MOD,
        POW,
        
        LPAREN,
        RPAREN,
        PIPE,
        BANG,
        COMMA,
        EQUALS,

        IDENTIFIER,
        NUMBER,

        END_OF_LINE
    };

    Kind kind;
    std::string value;
};

#endif