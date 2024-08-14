//
// Created by kiran on 5/21/24.
//

#ifndef KL_AST_EXPRESSIONS_H
#define KL_AST_EXPRESSIONS_H

#include "AST/AST_Preamble.h"
#include "codegen.h"
#include "symtab.h"
#include <llvm/IR/Value.h>

class ASTExpr : public ASTNode {
 private:
  static unique_ptr<ASTExpr> fold_binary(ASTExpr *);

  static unique_ptr<ASTExpr> fold_unary(ASTExpr *);

 public:
  ASTProgram *program;

  unsigned int line{};
  unsigned int col{};

  static unique_ptr<ASTExpr> fold(ASTExpr *);

  virtual KL_Type get_expr_type() { return KL_Type(); };

  virtual void print(int indent, ostream &out) const override {};

  virtual void check_semantics() override {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return Expr; }
};

// The primary expressions
class ASTExprConstantValue : public ASTExpr {
 public:
  ASTProgram *program;

  KL_Type type;
  string value;
  unsigned int line;
  unsigned int col;

  explicit ASTExprConstantValue(KL_Type type, string value, unsigned int line,
                                unsigned int col)
      : type(std::move(type)), value(std::move(value)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return ExprConstValue;
  }

  void check_semantics() override {}

  KL_Type get_expr_type() override { return KL_Type(type); };

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Value: " << value << std::endl;
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTExprIdentifier : public ASTExpr {
 public:
  ASTProgram *program;

  string name;
  unsigned int line;
  unsigned int col;

  explicit ASTExprIdentifier(string name, unsigned int line, unsigned int col)
      : name(std::move(name)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return ExprIdentifier;
  }

  KL_Type get_expr_type() override;

  void check_semantics() override;

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Identifier: " << name << std::endl;
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTExprFuncCall : public ASTExpr {
 public:
  ASTProgram *program;

  string name;
  unique_ptr<vector<unique_ptr<ASTExpr>>> args;
  unsigned int line;
  unsigned int col;

  explicit ASTExprFuncCall(string name,
                           unique_ptr<vector<unique_ptr<ASTExpr>>> args,
                           unsigned int line, unsigned int col)
      : name(std::move(name)), args(std::move(args)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return ExprFuncCall;
  }

  void fold_expressions() override {
    for (auto &arg : *args) {
      arg = ASTExpr::fold(arg.get());
    }
  }

  KL_Type get_expr_type() override {
    auto type = SYMTAB.get_name_type(name);
    if (!type) {
      throw std::runtime_error("Function " + name + " not found");
    }
    return type->get_return_type();
  }

  void check_semantics() override {
    for (const auto &arg : *args) {
      arg->check_semantics();
    }
  }

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);

    out << "Function Call: " << name << " : Args" << std::endl;
    for (const auto &arg : *args) {
      arg->print(indent + 1, out);
    }
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTExprBinary : public ASTExpr {
 private:
  // This is used to check if the expression has been checked for semantics
  // so that we don't check it again to get return type
  bool checked;
  KL_Type type;

 public:
  ASTProgram *program;

  unique_ptr<ASTExpr> lhs;
  unique_ptr<ASTExpr> rhs;
  KL_TokenType op;
  unsigned int line;
  unsigned int col;

  explicit ASTExprBinary(unique_ptr<ASTExpr> lhs, unique_ptr<ASTExpr> rhs,
                         KL_TokenType op, unsigned int line, unsigned int col)
      : lhs(std::move(lhs)),
        rhs(std::move(rhs)),
        op(op),
        line(line),
        col(col) {
          checked = false;
        }

  [[nodiscard]] ASTNodeType get_AST_type() const override { return ExprBinary; }

  void check_semantics() override;

  KL_Type get_expr_type() override;

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Binary Expression: " << token_type_to_string(op) << std::endl;
    printIndent(indent, out);
    out << "LHS:" << std::endl;
    lhs->print(indent + 1, out);
    printIndent(indent, out);
    out << "RHS:" << std::endl;
    rhs->print(indent + 1, out);
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTExprUnary : public ASTExpr {
 public:
  ASTProgram *program;

  unique_ptr<ASTExpr> expr;
  KL_TokenType op;
  unsigned int line;
  unsigned int col;

  explicit ASTExprUnary(KL_TokenType op, unique_ptr<ASTExpr> expr,
                        unsigned int line, unsigned int col)
      : expr(std::move(expr)), op(op), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return ExprUnary; }

  void check_semantics() override;

  KL_Type get_expr_type() override;

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Unary Expression: " << token_type_to_string(op) << std::endl;
    expr->print(indent + 1, out);
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

#endif  // KL_AST_EXPRESSIONS_H
