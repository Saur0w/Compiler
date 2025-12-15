#include "lexer.h"
#include <cctype>
#include <iostream>
#include <stdexcept>

Lexer::Lexer(std::string source) : source(std::move(source)), cursor(0), line(1), lineStart(0) {}

char Lexer::peek() const {
    if (cursor >= source.length()) return '\0';
    return source[cursor];
}

char Lexer::peekNext() const {
    if (cursor + 1 >= source.length()) return '\0';
    return source[cursor + 1];
}

char Lexer::advance() {
    if (cursor >= source.length()) return '\0';
    return source[cursor++];
}

void Lexer::addToken(TokenType type, std::string value) {
    Token token;
    token.type = type;
    token.value = std::move(value);
    token.loc.line = line;
    token.loc.col = (cursor - value.length()) - lineStart + 1;
    tokens.push_back(token);
}

std::vector<Token> Lexer::tokenize() {
    tokens.clear();

    while (cursor < source.length()) {
        char c = peek();

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;

            case '\n':
                line++;
                advance();
                lineStart = cursor;
                break;

            case '(': addToken(TokenType::LParen, "("); advance(); break;
            case ')': addToken(TokenType::RParen, ")"); advance(); break;
            case '{': addToken(TokenType::LBrace, "{"); advance(); break;
            case '}': addToken(TokenType::RBrace, "}"); advance(); break;
            case ',': addToken(TokenType::Comma, ","); advance(); break;
            case ';': addToken(TokenType::Semicolon, ";"); advance(); break;
            case '.': addToken(TokenType::Period, "."); advance(); break;


            case '+':
                if (peekNext() == '=') {
                    advance(); advance();
                    addToken(TokenType::Assign, "+=");
                } else if (peekNext() == '+') {
                    advance(); advance();
                    addToken(TokenType::Plus, "++");
                } else {
                    addToken(TokenType::Plus, "+");
                    advance();
                }
                break;

            case '-':
                if (peekNext() == '>') {
                    advance(); advance();
                } else {
                    addToken(TokenType::Minus, "-");
                    advance();
                }
                break;

            case '*': addToken(TokenType::Star, "*"); advance(); break;
            case '%': addToken(TokenType::Mod, "%"); advance(); break;

            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && peek() != '\0') advance();
                } else {
                    addToken(TokenType::Slash, "/"); advance();
                }
                break;

            case '=':
                if (peekNext() == '=') {
                    advance(); advance();
                    addToken(TokenType::Equal, "==");
                } else {
                    addToken(TokenType::Assign, "=");
                    advance();
                }
                break;

            case '!':
                if (peekNext() == '=') {
                    advance(); advance();
                    addToken(TokenType::NotEqual, "!=");
                } else {
                    addToken(TokenType::Not, "!");
                    advance();
                }
                break;

            case '<':
                if (peekNext() == '=') {
                    advance(); advance();
                    addToken(TokenType::LessThanEqual, "<=");
                } else {
                    addToken(TokenType::LessThan, "<");
                    advance();
                }
                break;

            case '>':
                if (peekNext() == '=') {
                    advance(); advance();
                    addToken(TokenType::GreaterThanEqual, ">=");
                } else {
                    addToken(TokenType::GreaterThan, ">");
                    advance();
                }
                break;

            case '"':
                scanString();
                break;

            default:
                if (isdigit(c)) {
                    scanNumber();
                } else if (isalpha(c) || c == '_') {
                    scanIdentifier();
                } else {
                    std::cerr << "Unexpected character: " << c << " at line " << line << "\n";
                    advance();
                }
                break;
        }
    }

    Token eof;
    eof.type = TokenType::Eof;
    eof.loc.line = line;
    tokens.push_back(eof);

    return tokens;
}

void Lexer::scanString() {
    advance();
    std::string value;

    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\n') line++;

        if (peek() == '\\') {
            advance();
            switch (peek()) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '"': value += '"'; break;
                case '\\': value += '\\'; break;
                default: value += peek(); break;
            }
        } else {
            value += peek();
        }
        advance();
    }

    if (peek() == '\0') {
        std::cerr << "Unterminated string at line " << line << "\n";
        return;
    }

    advance();
    addToken(TokenType::StringLiteral, value);
}

void Lexer::scanNumber() {
    size_t start = cursor;
    while (isdigit(peek())) addToken();

    if (peek() == '.' && isdigit(peekNext))
}