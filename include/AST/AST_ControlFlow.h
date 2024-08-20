//
// Created by kiran on 5/21/24.
//

#ifndef BASK_AST_CONTROLFLOW_H
#define BASK_AST_CONTROLFLOW_H

#include "AST_Statements.h"

class ASTControlFlowIf : public ASTStmt {
public:
  unique_ptr<ASTExpr> condition;
  unique_ptr<ASTBlock> then_block;
  unique_ptr<ASTBlock> else_block;
  unsigned line;
  unsigned col;

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return ControlFlowIf;
  }

  ASTControlFlowIf(unique_ptr<ASTExpr> condition,
                   unique_ptr<ASTBlock> then_block,
                   unique_ptr<ASTBlock> else_block, unsigned line, unsigned col)
      : condition(std::move(condition)), then_block(std::move(then_block)),
        else_block(std::move(else_block)), line(line), col(col) {}

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override {
    return v->visit(this);
  }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "If Statement:" << std::endl;
    condition->print(indent + 1, out);
    then_block->print(indent + 1, out);
    if (else_block) {
      printIndent(indent, out);
      out << "Else:" << std::endl;
      else_block->print(indent + 1, out);
    }
  }

  void check_semantics() override {
    condition->check_semantics();
    then_block->check_semantics();
    if (else_block) {
      else_block->check_semantics();
    }
  }

  void fold_expressions() override {
    condition = ASTExpr::fold(condition.get());
    then_block->fold_expressions();
    if (else_block) {
      else_block->fold_expressions();
    }
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTControlFlowWhile : public ASTStmt {
public:
  unique_ptr<ASTExpr> condition;
  unique_ptr<ASTBlock> block;
  unsigned line;
  unsigned col;

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return ControlFlowWhile;
  }

  ASTControlFlowWhile(unique_ptr<ASTExpr> condition, unique_ptr<ASTBlock> block,
                      unsigned line, unsigned col)
      : condition(std::move(condition)), block(std::move(block)), line(line),
        col(col) {}

  BASKCodeGenResult *accept(BASKCodeGenVisitor *v) override {
    return v->visit(this);
  }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "While Statement:" << std::endl;
    condition->print(indent + 1, out);
    block->print(indent + 1, out);
  }

  void check_semantics() override {
    condition->check_semantics();
    block->check_semantics();
  }

  void fold_expressions() override {
    condition = ASTExpr::fold(condition.get());
    block->fold_expressions();
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

#endif //BASK_AST_CONTROLFLOW_H
