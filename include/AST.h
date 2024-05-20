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

// --------------------------- Expressions ---------------------------
class ASTExpr : public ASTNode {};

class ASTExprLiteral : public ASTExpr {
public:
  KLTokenType type;
  string value;
  
  explicit ASTExprLiteral(KLTokenType type, string value) : type(type), value(std::move(value)) {}
};

class ASTExprIdentifier : public ASTExpr {
public:
  string name;
  
  explicit ASTExprIdentifier(string name) : name(std::move(name)) {}
};

class ASTExprBinary : public ASTExpr {
public:
  unique_ptr<ASTExpr> lhs;
  unique_ptr<ASTExpr> rhs;
  KLTokenType op;
  
  ASTExprBinary(unique_ptr<ASTExpr> lhs, unique_ptr<ASTExpr> rhs, KLTokenType op) : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}
};

class ASTExprUnary : public ASTExpr {
public:
  unique_ptr<ASTExpr> expr;
  KLTokenType op;
  
  ASTExprUnary(unique_ptr<ASTExpr> expr, KLTokenType op) : expr(std::move(expr)), op(op) {}
};

class ASTExprFuncCall : public ASTExpr {
public:
  string name;
  vector<unique_ptr<ASTExpr>> args;
  
  ASTExprFuncCall(string name, vector<unique_ptr<ASTExpr>> args) : name(std::move(name)), args(std::move(args)) {}
};

class ASTExprParen : public ASTExpr {
public:
  unique_ptr<ASTExpr> expr;
  
  explicit ASTExprParen(unique_ptr<ASTExpr> expr) : expr(std::move(expr)) {}
};

class ASTExprArrayAccess : public ASTExpr {
public:
  string name;
  unique_ptr<ASTExpr> index;
  
  ASTExprArrayAccess(string name, unique_ptr<ASTExpr> index) : name(std::move(name)), index(std::move(index)) {}
};

// --------------------------- Statements ---------------------------
class ASTStmt : public ASTNode {};

// used for expressions that are statements (e.g. function calls)
class ASTStmtExpr : public ASTStmt {
public:
  unique_ptr<ASTExpr> expr;
  
  explicit ASTStmtExpr(unique_ptr<ASTExpr> expr) : expr(std::move(expr)) {}

};

class ASTStmtDecl : public ASTStmt {
public:
  string name;
  KLTokenType type;
  unique_ptr<ASTExpr> value;
  
  ASTStmtDecl(string name, KLTokenType type, unique_ptr<ASTExpr> value) : name(std::move(name)), type(type), value(std::move(value)) {}
};

// --------------------------- Program ---------------------------
class ASTType : public ASTNode {
public:
  KL_Type type;
  
  explicit ASTType(KL_Type type) : type(std::move(type)) {}
};

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
