//
// Created by kiran on 5/20/24.
//

#ifndef KL_AST_H
#define KL_AST_H

#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "symtab.h"
#include "tokens.h"
#include "types.h"
#include "codegen.h"

#define SYMTAB ASTNode::symtab

using namespace std;

void printIndent(int indent, ostream &out);

class ASTNode {
 public:
  enum ASTNodeType {
    Type,

    Expr,
    ExprConstValue,
    ExprIdentifier,
    ExprFuncCall,
    ExprBinary,
    ExprUnary,

    Stmt,
    StmtExpr,
    StmtAssignment,
    StmtDecl,
    Block,

    ControlFlowIf,
    ControlFlowWhile,
    ControlFlowFor,
    ControlFlowReturn,
    ControlFlowContinue,
    ControlFlowBreak,
    Program,
    FuncDecl
  };

  static SymTab symtab;

  [[nodiscard]] virtual ASTNodeType get_AST_type() const = 0;

  unsigned int line;
  unsigned int col;

  virtual void fold_expressions() {}

  virtual void print(int indent, ostream &out) const = 0;

  virtual void check_semantics() {}

  virtual CodeGenResult accept(KLCodeGenVisitor *v) = 0;

  static void SyntaxError(unsigned int line, unsigned int col,
                          const string &message) {
    string msg = "Syntax Error at [" + to_string(line) + ", " + to_string(col) +
                 "]: " + message;
    throw std::runtime_error(msg);
  }

  // Variable already exists etc
  static void ValueError(unsigned int line, unsigned int col,
                         const string &message) {
    string msg = "Value Error at [" + to_string(line) + ", " + to_string(col) +
                  "]: " + message;
    throw std::runtime_error(msg);
  }

  // Type mismatch etc
  static void TypeError(unsigned int line, unsigned int col,
                        const string &message) {
    string msg = "Type Error at [" + to_string(line) + ", " + to_string(col) +
                  "]: " + message;
    throw std::runtime_error(msg);

  }

  // TODO add warning functions
};

class ASTType : public ASTNode {
 public:
  KL_Type type;
  unsigned int line;
  unsigned int col;

  explicit ASTType(KL_Type type, unsigned int line, unsigned int col)
      : type(std::move(type)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return Type; }

  CodeGenResult accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << type.to_string() << std::endl;
  }
};

class ASTProgram;

// --------------------------- Expressions ---------------------------
#include "AST_Expressions.h"

// --------------------------- Statements ---------------------------
#include "AST_Statements.h"

// ------------------------- Control Flow ------------------------
#include "AST_ControlFlow.h"

// --------------------------- Program ---------------------------
class ASTFuncDecl : public ASTNode {
 public:
  string name;
  unique_ptr<ASTType> returnType;
  unique_ptr<ASTBlock> body;
  vector<unique_ptr<ASTType>> argTypes;
  vector<string> argNames;
  unsigned int line;
  unsigned int col;
  ASTProgram *program;

  ASTFuncDecl(string name, unique_ptr<ASTType> returnType,
              vector<unique_ptr<ASTType>> argTypes, vector<string> argNames,
              unique_ptr<ASTBlock> body, unsigned int line, unsigned int col)
      : name(std::move(name)),
        returnType(std::move(returnType)),
        body(std::move(body)),
        line(line),
        col(col),
        argTypes(std::move(argTypes)),
        argNames(std::move(argNames)) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return FuncDecl; }

  void fold_expressions() override { body->fold_expressions(); }

  void check_semantics() override { body->check_semantics(); }

  CodeGenResult accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << returnType->type.to_string() << " function: " << name << std::endl;
    body->print(indent + 1, out);
  }
};

class ASTProgram : public ASTNode {
 protected:
  SymTab symtab;

 public:
  vector<unique_ptr<ASTFuncDecl>> funcs;

  explicit ASTProgram(vector<unique_ptr<ASTFuncDecl>> funcs)
      : funcs(std::move(funcs)) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return Program; }

  void fold_expressions() override {
    for (auto &func : funcs) {
      func->fold_expressions();
    }
  }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << "Program:" << std::endl;
    for (const auto &func : funcs) {
      func->print(indent + 1, out);
    }
  }

  void get_function_names(vector<string> &names) const {
    for (const auto &func : funcs) {
      names.push_back(func->name);
    }
  }

  void check_semantics() override {
    for (auto &func : funcs) {
      func->check_semantics();
    }
  }

  CodeGenResult accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  [[nodiscard]] const ASTFuncDecl *get_function(const string &name) const {
    for (const auto &func : funcs) {
      if (func->name == name) {
        return func.get();
      }
    }
    throw runtime_error("Function not found");
  }
};

#endif  // KL_AST_H
