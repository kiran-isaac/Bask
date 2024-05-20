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

class ASTNode {
public:
  virtual ~ASTNode() = default;
};

class ASTType : public ASTNode {
public:
  KL_Type type;
  
  explicit ASTType(KL_Type type) : type(std::move(type)) {}
};

// --------------------------- Expressions ---------------------------
class ASTExpr : public ASTNode {};

// Two types of expressions: primary and operator
class ASTExprPrimary : public ASTExpr {};
class ASTExprOperator : public ASTExpr {};

// The primary expressions
class ASTExprLiteral : public ASTExprPrimary {
public:
  KLTokenType type;
  string value;
  
  explicit ASTExprLiteral(KLTokenType type, string value) : type(type), value(std::move(value)) {}
};

class ASTExprIdentifier : public ASTExprPrimary {
public:
  string name;
  
  explicit ASTExprIdentifier(string name) : name(std::move(name)) {}
};

class ASTExprFuncCall : public ASTExprPrimary {
public:
  string name;
  unique_ptr<vector<unique_ptr<ASTExpr>>> args;
  
  ASTExprFuncCall(string name, unique_ptr<vector<unique_ptr<ASTExpr>>> args) : name(std::move(name)), args(std::move(args)) {}
};

class ASTExprParen : public ASTExprPrimary {
public:
  unique_ptr<ASTExpr> expr;
  
  explicit ASTExprParen(unique_ptr<ASTExpr> expr) : expr(std::move(expr)) {}
};

class ASTExprArrayAccess : public ASTExprPrimary {
public:
  string name;
  unique_ptr<ASTExpr> index;
  
  ASTExprArrayAccess(string name, unique_ptr<ASTExpr> index) : name(std::move(name)), index(std::move(index)) {}
};

// The operator expressions
class ASTExprBinary : public ASTExprOperator {
public:
  unique_ptr<ASTExpr> lhs;
  unique_ptr<ASTExpr> rhs;
  KLTokenType op;
  
  ASTExprBinary(unique_ptr<ASTExpr> lhs, unique_ptr<ASTExpr> rhs, KLTokenType op) : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}
};

class ASTExprUnary : public ASTExprOperator {
public:
  unique_ptr<ASTExpr> expr;
  KLTokenType op;
  
  ASTExprUnary(KLTokenType op, unique_ptr<ASTExpr> expr) : expr(std::move(expr)), op(op) {}
};

// --------------------------- Statements ---------------------------
class ASTStmt : public ASTNode {};

// used for expressions that are statements (e.g. function calls)
class ASTStmtExpr : public ASTStmt {
public:
  unique_ptr<ASTExpr> expr;
  
  explicit ASTStmtExpr(unique_ptr<ASTExpr> expr) : expr(std::move(expr)) {}

};

class ASTStmtAssignment : public ASTStmt {
public:
  bool is_const;
  unique_ptr<ASTType> type;
  string name;
  unique_ptr<ASTExpr> value;
  
  ASTStmtAssignment(bool is_const, unique_ptr<ASTType> type, string name, unique_ptr<ASTExpr> value) : is_const(is_const), type(std::move(type)), name(std::move(name)), value(std::move(value)) {}
};

class ASTStmtDecl : public ASTStmt {
public:
  string name;
  KLTokenType type;
  unique_ptr<ASTExpr> value;
  
  ASTStmtDecl(string name, KLTokenType type, unique_ptr<ASTExpr> value) : name(std::move(name)), type(type), value(std::move(value)) {}
};

// --------------------------- Program ---------------------------
class ASTFuncDecl : public ASTNode {
public:
  string name;
  unique_ptr<ASTType> returnType;
  vector<unique_ptr<ASTStmt>> body;
  
  ASTFuncDecl(string name, unique_ptr<ASTType> returnType, vector<unique_ptr<ASTStmt>> body) : name(std::move(name)), returnType(std::move(returnType)), body(std::move(body)) {}
};

class ASTProgram : public ASTNode {
public:
  vector<unique_ptr<ASTFuncDecl>> funcs;
  
  explicit ASTProgram(vector<unique_ptr<ASTFuncDecl>> funcs) : funcs(std::move(funcs)) {}
};

#endif //KL_AST_H
