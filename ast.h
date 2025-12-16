//
// Created by sthap on 16-12-2025.
//

#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct AstNode {
    virtual ~AstNode() = default;
};

struct Expr : public AstNode {};

struct NumberExpr : public AstNode {
    double value;
    NumberExpr(double v) : value(v) {}
};

struct StringExpr : public Expr {
    std::string value;
    StringExpr(std::string v) : value(v) {}
};

struct VariableExpr : public Expr {
    std::string name;
    VaraibleExpr(std::string n) : name(std::move(v)) {}
};

struct Stmt : public ASTNode {};

struct VarDeclStmt : public Stmt {
    std::string name;
    std::string type;
    std::unique_ptr<Expr> initializer;

    VarDeclStmt(std::string n, std::string t, std::unique_ptr<Expr> init)
        : name(std::move(n)), type(std::move(t)), initializer(std::move(init)) {}
};

struct FunctionStmt : public Stmt {
    std::string name;
    std::vector<std::unique_ptr<Stmt>> body;

    FunctionStmt(std::string n, std::vector<std::unique_ptr<Stmt>> b)
        : name(std::move(n)), body(std::move(b)) {}
};

struct BlockStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> s) : statements(std::move(s)) {}
};

#endif //COMPILER_AST_H