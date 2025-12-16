//
// Created by sthap on 16-12-2025.
//

#include "parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

Token Parser::peek() const { return tokens[current]; }
Token Parser::previous() const { return tokens[current - 1]; }
bool Parser::isAtEnd() const { return peek().type == TokenType::Eof; }

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, std::string message) {
    if (check(type)) return advance();
    throw std::runtime_error(message + " at line " + std::to_string(peek().loc.line));
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(TokenType::NumberLiteral)) {
        return std::make_unique<NumberExpr>(std::stod(previous().value));
    }
    if (match(TokenType::StringLiteral)) {
        return std::make_unique<StringExpr>(previous().value);
    }


    if (match(TokenType::Identifier)) {
        return
    }
}
