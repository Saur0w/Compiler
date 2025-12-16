#include "parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

// ============================================================
// HELPER FUNCTIONS
// ============================================================
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

// ============================================================
// MAIN PARSE LOOP
// ============================================================
std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        } catch (const std::runtime_error& error) {
            std::cerr << "Parse Error: " << error.what() << "\n";
            // Synchronize (skip to next valid statement to avoid cascading errors)
            advance();
        }
    }
    return statements;
}

// ============================================================
// DECLARATIONS (Top Level)
// ============================================================
std::unique_ptr<Stmt> Parser::declaration() {
    if (match(TokenType::Let)) return varDeclaration();
    if (match(TokenType::Function)) return funcDeclaration();
    return statement();
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    // let x : int = 10;
    Token name = consume(TokenType::Identifier, "Expect variable name.");

    std::string type = "inferred";
    if (match(TokenType::Colon)) { // Optional type annotation
        if (match(TokenType::Int)) type = "int";
        else if (match(TokenType::Float)) type = "float";
        else if (match(TokenType::Str)) type = "string";
        else consume(TokenType::Identifier, "Expect type name.");
    }

    std::unique_ptr<Expr> initializer = nullptr;
    if (match(TokenType::Assign)) {
        initializer = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after variable declaration.");
    return std::make_unique<VarDeclStmt>(name.value, type, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::funcDeclaration() {
    // fn name(params) { body }
    Token name = consume(TokenType::Identifier, "Expect function name.");
    consume(TokenType::LParen, "Expect '(' after function name.");
    // (We will handle parameters later to keep it simple for now)
    consume(TokenType::RParen, "Expect ')' after parameters.");

    consume(TokenType::LBrace, "Expect '{' before function body.");
    auto body = block(); // Parse the block { ... }

    // Convert BlockStmt to vector of stmts for the FunctionStmt constructor
    return std::make_unique<FunctionStmt>(name.value, std::move(body->statements));
}

// ============================================================
// STATEMENTS
// ============================================================
std::unique_ptr<Stmt> Parser::statement() {
    if (match(TokenType::LBrace)) return block();

    // If it's not a special statement, it must be an expression statement (like "x = 10;")
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::Semicolon, "Expect ';' after expression.");
    return nullptr; // In a real AST, you'd wrap this in an ExprStmt class
}

std::unique_ptr<BlockStmt> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RBrace) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RBrace, "Expect '}' after block.");
    return std::make_unique<BlockStmt>(std::move(statements));
}

// ============================================================
// EXPRESSIONS (Math Logic)
// ============================================================
std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();

    while (match(TokenType::Equal) || match(TokenType::NotEqual)) {
        std::string op = previous().value;
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();

    while (match(TokenType::GreaterThan) || match(TokenType::GreaterThanEqual) ||
           match(TokenType::LessThan) || match(TokenType::LessThanEqual)) {
        std::string op = previous().value;
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        std::string op = previous().value;
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = primary();
    while (match(TokenType::Star) || match(TokenType::Slash) || match(TokenType::Mod)) {
        std::string op = previous().value;
        std::unique_ptr<Expr> right = primary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(TokenType::NumberLiteral)) return std::make_unique<NumberExpr>(std::stod(previous().value));
    if (match(TokenType::StringLiteral)) return std::make_unique<StringExpr>(previous().value);
    if (match(TokenType::Identifier)) return std::make_unique<VariableExpr>(previous().value);

    if (match(TokenType::LParen)) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RParen, "Expect ')' after expression.");
        return expr;
    }

    throw std::runtime_error("Expect expression.");
}
