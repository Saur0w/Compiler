//
// Created by sthap on 16-12-2025.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<Stmt>> parse();
private:
    std::vector<Token> tokens;
    size_t current = 0;
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;
    bool match(TokenType type);
    Token consume(TokenType type, std::string message);
    bool check(TokenType type) const;
    Token advance();

    std::unique_ptr<Stmt> declaration();

    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Expr> funcDeclaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> primary();
};

#endif //COMPILER_PARSER_H