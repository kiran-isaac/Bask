//
// Created by kiran on 5/21/24.
//

#ifndef KL_AST_STATEMENTS_H
#define KL_AST_STATEMENTS_H

#include "AST.h"
#include "AST_Expressions.h"

class ASTStmt : public ASTNode {
 public:
  ASTProgram *program;

  virtual void fold_expressions() {}

  virtual void check_semantics() {}

  [[nodiscard]] virtual ASTNodeType get_AST_type() const = 0;

  virtual void print(int indent, ostream &out) const = 0;
};

class ASTStmtExpr : public ASTStmt {
 public:
  ASTProgram *program;

  unique_ptr<ASTExpr> expr;
  unsigned int line;
  unsigned int col;

  explicit ASTStmtExpr(unique_ptr<ASTExpr> expr, unsigned int line,
                       unsigned int col)
      : expr(std::move(expr)), line(line), col(col) {}

  void fold_expressions() override { expr = ASTExpr::fold(expr.get()); }

  void check_semantics() override { expr->check_semantics(); }

  [[nodiscard]] ASTNodeType get_AST_type() const override { return StmtExpr; }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Expression Statement:" << std::endl;
    expr->print(indent + 1, out);
  };
};

class ASTStmtAssignment : public ASTStmt {
 public:
  ASTProgram *program;

  string name;
  unique_ptr<ASTExpr> value;
  unsigned int line;
  unsigned int col;

  ASTStmtAssignment(string name, unique_ptr<ASTExpr> value, unsigned int line,
                    unsigned int col)
      : name(std::move(name)), value(std::move(value)), line(line), col(col) {}

  void fold_expressions() override { value = ASTExpr::fold(value.get()); }

  void check_semantics() override;

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return StmtAssignment;
  }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Assignment: " << name << std::endl;
    value->print(indent + 1, out);
  }
};

class ASTStmtDecl : public ASTStmt {
 public:
  ASTProgram *program;
  unique_ptr<ASTType> type;
  string name;
  unique_ptr<ASTExpr> value;
  unsigned int line;
  unsigned int col;

  ASTStmtDecl(unique_ptr<ASTType> type, string name, unique_ptr<ASTExpr> value,
              unsigned int line, unsigned int col)
      : type(std::move(type)),
        name(std::move(name)),
        value(std::move(value)),
        line(line),
        col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return StmtDecl; }

  void fold_expressions() override { value = ASTExpr::fold(value.get()); }

  void check_semantics() override;

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Declaration: " << name << std::endl;
    type->print(indent + 1, out);
    if (value) {
      value->print(indent + 1, out);
    }
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

  void check_semantics() {
    for (auto &stmt : body) {
      stmt->check_semantics();
    }
  }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Block:" << std::endl;
    for (const auto &stmt : body) {
      stmt->print(indent + 1, out);
    }
  }
};

#endif  // KL_AST_STATEMENTS_H
