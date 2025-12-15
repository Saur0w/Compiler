//
// Created by sthap on 15-12-2025.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <string>
#include <vector>
#include <string_view>
#include <iostream>


enum class TokenType {
    Eof,

    // Delimiters
    LParen, RParen,
    LBrace, RBrace,
    Comma, Period, Semicolon,

    // Operators
    Plus, Minus, Star, Slash, Mod,
    Assign,
    Equal,
    NotEqual,
    LessThan, GreaterThan,
    LessThanEqual, GreaterThanEqual,
    And, Or, Not, Xor,

    // Keywords
    Function, Return,
    If, Else, While,
    Type, Let,
    True, False,

    // Variable Types (Keywords)
    Int, Float, Bool, Str,
    Short, Long, Signed, Unsigned,

    // Literals
    Identifier,
    NumberLiteral,
    StringLiteral
};

struct Location {
    size_t line = 1;
    size_t col = 1;
};

struct Token {
    TokenType type;
    std::string value;
    Location loc;
    bool is(TokenType t) const {return type == t;}
};

class Lexer {
public:
    explicit Lexer(std::string source);

    std::vector<Token> tokenize();

    static std::string tokenToString(TokenType type);

private:
    std::string source;
    std::vector<Token> tokens;
    size_t cursor = 0;
    size_t line = 1;
    size_t lineStart = 0;

    char advance();
    char peek() const;
    char peekNext() const;

    void addToken(TokenType type, std::string value);
    void skipWhitespace();

    void scanIdentifier();
    void scanNumber();
    void scanString();

};

#endif //COMPILER_LEXER_H