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
#include "lexer.h"
#include "tokens.h"
#include "types.h"

using namespace std;

void printIndent(int indent);


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
  
  virtual void foldExpr() {}
  
  [[nodiscard]] virtual ASTNodeType getAstType() const = 0;
  
  unsigned int line = 0;
  unsigned int col = 0;
  
  virtual void print(int indent) const = 0;
  
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
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Type: %s\n", type_to_string(type).c_str());
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
                                                                                           value(std::move(value)),line(line), col(col) {}
  
                                                                                           [[nodiscard]] ASTNodeType getAstType() const override {
    return ExprValue;
  }
  
  void print(int indent) const override {
    printIndent(indent);
    printf("%s: %s\n", type_to_string(type).c_str(), value.c_str());
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
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Identifier: %s\n", name.c_str());
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
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Function Call: \"%s\" Args:\n", name.c_str());
    
    for (const auto &arg: *args) {
      arg->print(indent + 1);
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
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Binary Expression: %s\n", tokenTypeToString(op));
    printIndent(indent);
    printf("LHS\n");
    lhs->print(indent + 1);
    printIndent(indent);
    printf("RHS\n");
    rhs->print(indent + 1);
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
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Unary Expression: %s\n", tokenTypeToString(op));
    expr->print(indent + 1);
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
  
  void foldExpr() override {
    expr = ASTExpr::fold(expr.get());
  }
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Expression Statement\n");
    expr->print(indent + 1);
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
  
  void foldExpr() override {
    value = ASTExpr::fold(value.get());
  }
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Assignment: %s\n", name.c_str());
    value->print(indent + 1);
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
              unsigned int col): type(std::move(type)), name(std::move(name)),
                                  value(std::move(value)), line(line), col(col) {}
  
  [[nodiscard]] ASTNodeType getAstType() const override {
    return StmtDecl;
  }
  
  void foldExpr() override {
    value = ASTExpr::fold(value.get());
  }
  
  void print(int indent) const override {
    printIndent(indent);
    printf("Declaration: %s %s\n", type_to_string(type->type).c_str(), name.c_str());
    value->print(indent + 1);
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
  
  void foldExpr() override {
    for (auto &stmt: body) {
      stmt->foldExpr();
    }
  }
  
  void print(int indent) const override {
    printIndent(indent);
    printf("%s Function: %s\n", type_to_string(returnType->type).c_str(), name.c_str());
    
    for (const auto &stmt: body) {
      stmt->print(indent + 1);
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
  
  void foldExpr() override {
    for (auto &func: funcs) {
      func->foldExpr();
    }
  }
  
  void print(int indent) const override {
    printf("Program:\n");
    
    for (const auto &func: funcs) {
      func->print(indent + 1);
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
