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
  virtual ~ASTNode() = default;


  virtual void print(int indent) const = 0;
};

class ASTType : public ASTNode {
public:
  KL_Type type;
  
  explicit ASTType(KL_Type type) : type(std::move(type)) {}


void print(int indent) const override {
    printIndent(indent);
    printf("Type: %s\n", type_to_string(type).c_str());
  }
};

// --------------------------- Expressions ---------------------------
class ASTExpr : public ASTNode {
};

// Two types of expressions: primary and operator
class ASTExprPrimary : public ASTExpr {};
class ASTExprOperator : public ASTExpr {};

// The primary expressions
class ASTExprLiteral : public ASTExprPrimary {
public:
  KLTokenType type;
  string value;
  
  explicit ASTExprLiteral(KLTokenType type, string value) : type(type), value(std::move(value)) {}


void print(int indent) const override {
    printIndent(indent);
    printf("%s: %s\n", tokenTypeToString(type), value.c_str());
  }
};

class ASTExprIdentifier : public ASTExprPrimary {
public:
  string name;
  
  explicit ASTExprIdentifier(string name) : name(std::move(name)) {}



  void print(int indent) const override {
    printIndent(indent);
    printf("Identifier: %s\n", name.c_str());
  }
};

class ASTExprFuncCall : public ASTExprPrimary {
public:
  string name;
  unique_ptr<vector<unique_ptr<ASTExpr>>> args;
  
  ASTExprFuncCall(string name, unique_ptr<vector<unique_ptr<ASTExpr>>> args) : name(std::move(name)), args(std::move(args)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Function Call: \"%s\" Args:\n", name.c_str());
    
    for (const auto &arg : *args) {
      arg->print(indent + 1);
    }
  }
};

class ASTExprParen : public ASTExprPrimary {
public:
  unique_ptr<ASTExpr> expr;
  
  explicit ASTExprParen(unique_ptr<ASTExpr> expr) : expr(std::move(expr)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Parenthesized Expression:\n");
    expr->print(indent + 1);
  }
};

class ASTExprArrayAccess : public ASTExprPrimary {
public:
  string name;
  unique_ptr<ASTExpr> index;
  
  ASTExprArrayAccess(string name, unique_ptr<ASTExpr> index) : name(std::move(name)), index(std::move(index)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Array Access: %s\n", name.c_str());
    index->print(indent + 1);
  }
};

// The operator expressions
class ASTExprBinary : public ASTExprOperator {
public:
  unique_ptr<ASTExpr> lhs;
  unique_ptr<ASTExpr> rhs;
  KLTokenType op;
  
  ASTExprBinary(unique_ptr<ASTExpr> lhs, unique_ptr<ASTExpr> rhs, KLTokenType op) : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}


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

class ASTExprUnary : public ASTExprOperator {
public:
  unique_ptr<ASTExpr> expr;
  KLTokenType op;
  
  ASTExprUnary(KLTokenType op, unique_ptr<ASTExpr> expr) : expr(std::move(expr)), op(op) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Unary Expression: %s\n", tokenTypeToString(op));
    expr->print(indent + 1);
  }
};

// --------------------------- Statements ---------------------------
class ASTStmt : public ASTNode {
public:

  virtual string getInfo() const = 0;
  virtual ASTExpr *getExpr() const { return nullptr; }
};

// used for expressions that are statements (e.g. function calls)
class ASTStmtExpr : public ASTStmt {
public:
  unique_ptr<ASTExpr> expr;
  
  explicit ASTStmtExpr(unique_ptr<ASTExpr> expr) : expr(std::move(expr)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Expression Statement\n");
    expr->print(indent + 1);
  }
  
  [[nodiscard]] string getInfo() const override {
    return "Expression";
  }
  
  [[nodiscard]] ASTExpr *getExpr() const override {
    return expr.get();
  }
};

class ASTStmtAssignment : public ASTStmt {
public:
  string name;
  unique_ptr<ASTExpr> value;
  
  ASTStmtAssignment(string name, unique_ptr<ASTExpr> value) : name(std::move(name)), value(std::move(value)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Assignment: %s\n", name.c_str());
    value->print(indent + 1);
  }
  
  [[nodiscard]] string getInfo() const override {
    return "Assignment";
  }
  
  [[nodiscard]] ASTExpr *getExpr() const override {
    return value.get();
  }
};

class ASTStmtDecl : public ASTStmt {
public:
  bool is_const;
  unique_ptr<ASTType> type;
  string name;
  unique_ptr<ASTExpr> value;
  
  ASTStmtDecl(bool is_const, unique_ptr<ASTType> type, string name, unique_ptr<ASTExpr> value) : is_const(is_const), type(std::move(type)), name(std::move(name)), value(std::move(value)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("Declaration: %s %s\n", type_to_string(type->type).c_str(), name.c_str());
    value->print(indent + 1);
  }
  
  [[nodiscard]] string getInfo() const override {
    return "Declaration";
  }
  
  [[nodiscard]] ASTExpr *getExpr() const override {
    return value.get();
  }
};

// --------------------------- Program ---------------------------
class ASTFuncDecl : public ASTNode {
public:
  string name;
  unique_ptr<ASTType> returnType;
  vector<unique_ptr<ASTStmt>> body;
  
  ASTFuncDecl(string name, unique_ptr<ASTType> returnType, vector<unique_ptr<ASTStmt>> body) : name(std::move(name)), returnType(std::move(returnType)), body(std::move(body)) {}


  void print(int indent) const override {
    printIndent(indent);
    printf("%s Function: %s\n", type_to_string(returnType->type).c_str(), name.c_str());
    
    for (const auto &stmt : body) {
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


  void print(int indent) const override {
    printf("Program:\n");
    
    for (const auto &func : funcs) {
      func->print(indent + 1);
    }
  }
  
  void getFunctionNames(vector<string> &names) const {
    for (const auto &func : funcs) {
      names.push_back(func->name);
    }
  }
  
  [[nodiscard]] const ASTFuncDecl *getFunction(const string &name) const {
    for (const auto &func : funcs) {
      if (func->name == name) {
        return func.get();
      }
    }
    throw runtime_error("Function not found");
  }
};

#endif //KL_AST_H
