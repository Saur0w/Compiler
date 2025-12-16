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

            case ':': addToken(TokenType::Colon, ":"); advance(); break;

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
    while (isdigit(peek())) advance();

    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek())) advance();
    }

    std::string value = source.substr(start, cursor - start);
    addToken(TokenType::NumberLiteral, value);
}

void Lexer::scanIdentifier() {
    size_t start = cursor;
    while (isalnum(peek())  || peek() == '_') advance();

    std::string text = source.substr(start, cursor - start);
    TokenType type = TokenType::Identifier;

    if (text == "fn") type = TokenType::Function;
    else if (text == "return") type = TokenType::Return;
    else if (text == "let") type = TokenType::Let;
    else if (text == "true") type = TokenType::True;
    else if (text == "false") type = TokenType::False;
    else if (text == "if") type = TokenType::If;
    else if (text == "else") type = TokenType::Else;
    else if (text == "while") type = TokenType::While;

    //data types
    else if (text == "int") type = TokenType::Int;
    else if (text == "float") type = TokenType::Float;
    else if (text == "str") type = TokenType::Str;

    addToken(type, text);
}

std::string Lexer::tokenToString(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "Identifier";
        case TokenType::NumberLiteral: return "Number";
        case TokenType::StringLiteral: return "String";
        case TokenType::Function: return "Function";
        case TokenType::Return: return "Return";
        case TokenType::Let: return "Let";
        case TokenType::Int: return "Int";


        default: return "Token";
    }
}