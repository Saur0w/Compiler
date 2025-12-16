#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct Expr : public ASTNode {};

struct NumberExpr : public Expr {
    double value;
    explicit NumberExpr(double v) : value(v) {}
};

struct StringExpr : public Expr {
    std::string value;
    explicit StringExpr(std::string v) : value(std::move(v)) {}
};

struct VariableExpr : public Expr {
    std::string name;
    explicit VariableExpr(std::string n) : name(std::move(n)) {}
};


struct BinaryExpr : public Expr {
    std::unique_ptr<Expr> left;
    std::string op; 
    std::unique_ptr<Expr> right;
    
    BinaryExpr(std::unique_ptr<Expr> l, std::string o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
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
    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> s) : statements(std::move(s)) {}
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expression;
    explicit ExprStmt(std::unique_ptr<Expr> expr) : expression(std::move(expr)) {}
};

#endif

