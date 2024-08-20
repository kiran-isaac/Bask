//
// Created by kiran on 5/21/24.
//

#ifndef BASK_AST_STATEMENTS_H
#define BASK_AST_STATEMENTS_H

#include "AST/AST_Expressions.h"
#include "AST/AST_Preamble.h"
#include "codegen.h"
#include "types.h"
#include <memory>

class ASTStmt : public ASTNode {
public:
  virtual void fold_expressions() {}

  virtual void check_semantics() {}

  [[nodiscard]] virtual ASTNodeType get_AST_type() const = 0;

  virtual void print(int indent, ostream &out) const = 0;

  virtual BASKCodeGenResult *accept(BASKCodeGenVisitor *v) = 0;
};

class ASTStmtExpr : public ASTStmt {
 public:
  ASTProgram *program;

  unique_ptr<ASTExpr> expr;
  unsigned int line;
  unsigned int col;

  ASTStmtExpr(unique_ptr<ASTExpr> expr, unsigned int line, unsigned int col)
      : expr(std::move(expr)), line(line), col(col) {}

  void fold_expressions() override { expr = ASTExpr::fold(expr.get()); }

  void check_semantics() override { expr->check_semantics(); }

  [[nodiscard]] ASTNodeType get_AST_type() const override { return StmtExpr; }

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Expression Statement:" << std::endl;
    expr->print(indent + 1, out);
  };

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTStmtAssignment : public ASTStmt {
 public:
  ASTProgram *program;

  unique_ptr<ASTExprIdentifier> identifier;
  unique_ptr<ASTExpr> value;
  unsigned int line;
  unsigned int col;

  ASTStmtAssignment(string name, unique_ptr<ASTExpr> value, unsigned int line,
                    unsigned int col)
      : identifier(std::make_unique<ASTExprIdentifier>(name, line, col)),
        value(std::move(value)), line(line), col(col) {}

  void fold_expressions() override { value = ASTExpr::fold(value.get()); }

  void check_semantics() override;

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return StmtAssignment;
  }

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Assignment: " << identifier->name << std::endl;
    value->print(indent + 1, out);
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTStmtDecl : public ASTStmt {
 public:
  ASTProgram *program;
  unique_ptr<ASTType> type;
  unique_ptr<ASTExprIdentifier> identifier;
  unique_ptr<ASTExpr> value;
  unsigned int line;
  unsigned int col;

  ASTStmtDecl(unique_ptr<ASTType> type, string name, unique_ptr<ASTExpr> value,
              unsigned int line, unsigned int col)
      : type(std::move(type)), identifier(std::make_unique<ASTExprIdentifier>(name, line, col)),
        value(std::move(value)), line(line), col(col) {}

  ASTStmtDecl(BASK_Type type, string name, unique_ptr<ASTExpr> value,
              unsigned int line, unsigned int col)
      : type(std::make_unique<ASTType>(type, line, col)),
        identifier(std::make_unique<ASTExprIdentifier>(name, line, col)),
        value(std::move(value)), line(line), col(col) {}

  ASTStmtDecl(BASK_Type type, string name,
              unsigned int line, unsigned int col)
      : type(std::make_unique<ASTType>(type, line, col)),
        identifier(std::make_unique<ASTExprIdentifier>(name, line, col)),
        value(nullptr), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return StmtDecl; }

  void fold_expressions() override { value = ASTExpr::fold(value.get()); }

  void check_semantics() override;

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Declaration: " << identifier->name << std::endl;
    type->print(indent + 1, out);
    if (value) {
      value->print(indent + 1, out);
    }
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTStmtReturn : public ASTStmt {
public:
  ASTProgram *program;

  unique_ptr<ASTExpr> return_expr;
  unsigned int line;
  unsigned int col;

  ASTStmtReturn(unique_ptr<ASTExpr> return_expr, unsigned line, unsigned col)
      : return_expr(std::move(return_expr)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return StmtReturn; }

  void fold_expressions() override { return_expr = ASTExpr::fold(return_expr.get()); }

  void check_semantics() override { return_expr->check_semantics(); }

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Return Statement:" << std::endl;
    return_expr->print(indent + 1, out);
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTBlock : public ASTNode {
 public:
  ASTProgram *program;
  vector<unique_ptr<ASTStmt>> body;
  unsigned int line;
  unsigned int col;

  ASTBlock(vector<unique_ptr<ASTStmt>> body, unsigned int line,
           unsigned int col)
      : body(std::move(body)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return Block; }

  void fold_expressions() override {
    for (auto &stmt : body) {
      stmt->fold_expressions();
    }
  }

  ASTStmt *get_statement(int i) { return body[i].get(); }

  int size() { return body.size(); }

  void check_semantics() override {
    for (auto &stmt : body) {
      stmt->check_semantics();
    }
  }

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Block:" << std::endl;
    for (const auto &stmt : body) {
      stmt->print(indent + 1, out);
    }
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

#endif  // BASK_AST_STATEMENTS_H
