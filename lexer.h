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

    // Delimiters
    Plus, Minus, Star, Slash, Mod,
    Assign,
    Equal,
    LessThan, GreaterThan,
    LessThanEqual, GreaterThanEqual,
    And, Or, Not, Xor,
};

typedef struct location {
    size_t line;
    size_t col;
} Location;

typedef struct token_data {
    Token type;
    char *val;
    Location loc;
} TokenData;

typedef struct lexer {
    char *start_tok;
    char *cur_tok;
    TokenData *tokens;
    size_t line_number;
    char *line_start;
    size_t token_count;
    size_t capacity;

} Lexer;

int lex(Lexer *lexer);

void free_lexer(Lexer *lexer);

char *token_to_string(Token token);

#endif //COMPILER_LEXER_H