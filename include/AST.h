//
// Created by kiran on 5/20/24.
//

#ifndef KL_AST_H
#define KL_AST_H

#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include "lexer.h"
#include "tokens.h"
#include "types.h"

using namespace std;

void printIndent(int indent, ostream &out);

class ASTNode {
public:
  enum ASTNodeType {
    Root,
    Type,
    Expr,
    ExprValue,
    ExprIdentifier,
    ExprFuncCall,
    ExprBinary,
    ExprUnary,
    Stmt,
    StmtExpr,
    StmtAssignment,
    StmtDecl,
    Program,
    FuncDecl
  };
  
  virtual ~ASTNode() = default;
  
  virtual void foldExpressions() {}
  
  [[nodiscard]] virtual ASTNodeType getAstType() const = 0;
  
  unsigned int line = 0;
  unsigned int col = 0;
  
  virtual void print(int indent, ostream &out) const = 0;
  
  static void SyntaxError(ASTNode *node, const string &message) {
    printf("Syntax Error at [%d, %d]: %s", node->line, node->col, message.c_str());
    exit(1);
  }
};

class ASTType : public ASTNode {
public:
  KL_Type type;
  unsigned int line;
  unsigned int col;
  
  explicit ASTType(KL_Type type, unsigned int line, unsigned int col) : type(std::move(type)), line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return Type;
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << type_to_string(type) << std::endl;
  }
};

// --------------------------- Expressions ---------------------------
class ASTExpr : public ASTNode {
private:
  static unique_ptr<ASTExpr> foldBinary(ASTExpr *);
  
  static unique_ptr<ASTExpr> foldUnary(ASTExpr *);

public:
  unsigned int line{};
  unsigned int col{};
  
  static unique_ptr<ASTExpr> fold(ASTExpr *);
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return Expr;
  }
};


// The primary expressions
class ASTExprValue : public ASTExpr {
public:
  KL_Type type;
  string value;
  unsigned int line;
  unsigned int col;
  
  explicit ASTExprValue(KL_Type type, string value, unsigned int line, unsigned int col) : type(std::move(type)),
                                                                                           value(std::move(value)),
                                                                                           line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return ExprValue;
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Value: " << value << std::endl;
  }
};

class ASTExprIdentifier : public ASTExpr {
public:
  string name;
  unsigned int line;
  unsigned int col;
  
  explicit ASTExprIdentifier(string name, unsigned int line, unsigned int col) : name(std::move(name)), line(line),
                                                                                 col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return ExprIdentifier;
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Identifier: " << name << std::endl;
  }
};

class ASTExprFuncCall : public ASTExpr {
public:
  string name;
  unique_ptr<vector<unique_ptr<ASTExpr>>> args;
  unsigned int line;
  unsigned int col;
  
  ASTExprFuncCall(string name, unique_ptr<vector<unique_ptr<ASTExpr>>> args, unsigned int line, unsigned int col)
    : name(std::move(name)), args(std::move(args)), line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return ExprFuncCall;
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    
    out << "Function Call: " << name << " : Args" << std::endl;
    for (const auto &arg: *args) {
      arg->print(indent + 1, out);
    }
  }
};

// The operator expressions
class ASTExprBinary : public ASTExpr {
public:
  unique_ptr<ASTExpr> lhs;
  unique_ptr<ASTExpr> rhs;
  KLTokenType op;
  unsigned int line;
  unsigned int col;
  
  ASTExprBinary(unique_ptr<ASTExpr> lhs, unique_ptr<ASTExpr> rhs, KLTokenType op, unsigned int line, unsigned int col)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op), line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return ExprBinary;
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Binary Expression: " << tokenTypeToString(op) << std::endl;
    printIndent(indent, out);
    out << "LHS:" << std::endl;
    lhs->print(indent + 1, out);
    printIndent(indent, out);
    out << "RHS:" << std::endl;
    rhs->print(indent + 1, out);
  }
};

class ASTExprUnary : public ASTExpr {
public:
  unique_ptr<ASTExpr> expr;
  KLTokenType op;
  unsigned int line;
  unsigned int col;
  
  ASTExprUnary(KLTokenType op, unique_ptr<ASTExpr> expr, unsigned int line, unsigned int col) : expr(std::move(expr)),
                                                                                                op(op), line(line),
                                                                                                col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return ExprUnary;
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Unary Expression: " << tokenTypeToString(op) << std::endl;
    expr->print(indent + 1, out);
  }
};

// --------------------------- Statements ---------------------------
class ASTStmt : public ASTNode {
public:
  unsigned int line = 0;
  unsigned int col = 0;
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return Stmt;
  }
};

// used for expressions that are statements (e.g. function calls)
class ASTStmtExpr : public ASTStmt {
public:
  unique_ptr<ASTExpr> expr;
  unsigned int line;
  unsigned int col;
  
  explicit ASTStmtExpr(unique_ptr<ASTExpr> expr, unsigned int line, unsigned int col) : expr(std::move(expr)),
                                                                                        line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return StmtExpr;
  }
  
  void foldExpressions() override {
    expr = ASTExpr::fold(expr.get());
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Expression Statement:" << std::endl;
    expr->print(indent + 1, out);
  }
};

class ASTStmtAssignment : public ASTStmt {
public:
  string name;
  unique_ptr<ASTExpr> value;
  unsigned int line;
  unsigned int col;
  
  ASTStmtAssignment(string name, unique_ptr<ASTExpr> value, unsigned int line, unsigned int col) : name(
    std::move(name)), value(std::move(value)), line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return StmtAssignment;
  }
  
  void foldExpressions() override {
    value = ASTExpr::fold(value.get());
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Assignment: " << name << std::endl;
    value->print(indent + 1, out);
  }
};

class ASTStmtDecl : public ASTStmt {
public:
  unique_ptr<ASTType> type;
  string name;
  unique_ptr<ASTExpr> value;
  unsigned int line;
  unsigned int col;
  
  ASTStmtDecl(unique_ptr<ASTType> type, string name, unique_ptr<ASTExpr> value, unsigned int line,
              unsigned int col) : type(std::move(type)), name(std::move(name)),
                                  value(std::move(value)), line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return StmtDecl;
  }
  
  void foldExpressions() override {
    value = ASTExpr::fold(value.get());
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Declaration: " << name << std::endl;
    type->print(indent + 1, out);
    if (value) {
      value->print(indent + 1, out);
    }
  }
};

// --------------------------- Program ---------------------------
class ASTFuncDecl : public ASTNode {
public:
  string name;
  unique_ptr<ASTType> returnType;
  vector<unique_ptr<ASTStmt>> body;
  unsigned int line;
  unsigned int col;
  
  ASTFuncDecl(string name, unique_ptr<ASTType> returnType, vector<unique_ptr<ASTStmt>> body, unsigned int line,
              unsigned int col) : name(std::move(name)), returnType(std::move(returnType)), body(std::move(body)),
                                  line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return FuncDecl;
  }
  
  void foldExpressions() override {
    for (auto &stmt: body) {
      stmt->foldExpressions();
    }
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << type_to_string(returnType->type) << " function: " << name << std::endl;
    for (const auto &stmt: body) {
      stmt->print(indent + 1, out);
    }
  }
  
  [[nodiscard]] ASTStmt *getStatement(int index) const {
    if (index >= body.size()) {
      throw runtime_error("debugging function access statement index out of bounds");
    }
    return body[index].get();
  }
};

class ASTProgram : public ASTNode {
public:
  vector<unique_ptr<ASTFuncDecl>> funcs;
  
  explicit ASTProgram(vector<unique_ptr<ASTFuncDecl>> funcs) : funcs(std::move(funcs)) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return Program;
  }
  
  void foldExpressions() override {
    for (auto &func: funcs) {
      func->foldExpressions();
    }
  }
  
  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Program:" << std::endl;
    for (const auto &func: funcs) {
      func->print(indent + 1, out);
    }
  }
  
  void getFunctionNames(vector<string> &names) const {
    for (const auto &func: funcs) {
      names.push_back(func->name);
    }
  }
  
  [[nodiscard]] const ASTFuncDecl *getFunction(const string &name) const {
    for (const auto &func: funcs) {
      if (func->name == name) {
        return func.get();
      }
    }
    throw runtime_error("Function not found");
  }
};

#endif //KL_AST_H
