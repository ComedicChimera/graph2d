#include "lexer.hpp"

#include <stdexcept>
#include <iostream>

Token Lexer::nextToken() {
    while (peek()) {
        switch (ahead) {
        case '+':
            read();
            return makeToken(Token::Kind::PLUS);
        case '-':
            read();
            return makeToken(Token::Kind::MINUS);
        case '*':
            read();
            return makeToken(Token::Kind::TIMES);
        case '/':
            read();
            return makeToken(Token::Kind::DIV);
        case '%':
            read();
            return makeToken(Token::Kind::MOD);
        case '^':
            read();
            return makeToken(Token::Kind::POW);
        case '(':
            read();
            return makeToken(Token::Kind::LPAREN);
        case ')':
            read();
            return makeToken(Token::Kind::RPAREN);
        case '=':
            read();
            return makeToken(Token::Kind::EQUALS);
        case '|':
            read();
            return makeToken(Token::Kind::PIPE);
        case '!':
            read();
            return makeToken(Token::Kind::BANG);
        case ',':
            read();
            return makeToken(Token::Kind::COMMA);
        default:
            if (isdigit(ahead) || ahead == '.') {
                return lexNumber();
            } else if (isalpha(ahead)) {
                return lexIdentifier();
            } else {
                skip();
            }
        }
    }

    return Token{Token::Kind::END_OF_LINE};
}

/* -------------------------------------------------------------------------- */

Token Lexer::lexNumber() {
    if (ahead == '.') {
        tokBuff.push_back('0');
        read();

        while (peek() && isdigit(ahead))
            read();

        if (tokBuff.length() == 2) {
            throw std::logic_error("expected digit");
        }
    } else {
        read();

        bool hasDecimal = false;
        while (peek()) {
            

            if (isdigit(ahead))
                read();
            else if (ahead == '.') {
                if (hasDecimal)
                    throw std::logic_error("multiple decimals in a number");
                else {
                    read();
                    hasDecimal = true;
                }
            } else {
                break;
            }
        }
    }

    return makeToken(Token::Kind::NUMBER);
}

Token Lexer::lexIdentifier() {
    read();

    while (peek() && isalpha(ahead)) {
        read();
    }

    return makeToken(Token::Kind::IDENTIFIER);
}

/* -------------------------------------------------------------------------- */

Token Lexer::makeToken(Token::Kind kind) {
    Token tok { kind, tokBuff };

    tokBuff.clear();

    return tok;
}

bool Lexer::read() {
    if (ndx == src.length())
        return false;

    char curr = src[ndx++];
    tokBuff.push_back(curr);
    return true;
}

bool Lexer::skip() {
    if (ndx == src.length())
        return false;

    ndx++;
    return true;
}

bool Lexer::peek() {
    if (ndx == src.length())
        return false;

    ahead = src[ndx];
    return true;
}