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

#include "AST_Preamble.h"

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

  void fold_expressions() override { if (body) body->fold_expressions(); }

  vector<KL_Type> get_signature() const {
    vector<KL_Type> signature;
    for (auto &argType : argTypes) {
      signature.push_back(argType->type);
    }
    signature.push_back(returnType->type);
    return signature;
  }

  KL_Type get_type() const {
    return KL_Type(returnType->type.is_const, get_signature());
  }

  void check_semantics() override {
    if (!body)
      return;

    SYMTAB.add_name(name, get_type());
    SYMTAB.enter_block();
    for (size_t i = 0; i < argTypes.size(); i++) {
      SYMTAB.add_name(argNames[i], argTypes[i]->type);
    }
    body->check_semantics(); 
  }

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << returnType->type.to_string() << " function: " << name << std::endl;
    body->print(indent + 1, out);
  }

  std::string positionString() override {
    return "[" + to_string(line) + ", " + to_string(col) + "]";
  }
};

class ASTProgram : public ASTNode {
 protected:
  SymTab symtab;

 public:
  vector<unique_ptr<ASTFuncDecl>> funcs;

  explicit ASTProgram() {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return Program; }

  void fold_expressions() override {
    for (auto &func : funcs) {
      func->fold_expressions();
    }
  }

  bool add_function(unique_ptr<ASTFuncDecl> func) {
    // Check if function already exists
    for (const auto &f : funcs) {
      if (f->name == func->name) {
        return false;
      }
    }

    funcs.push_back(std::move(func));
    return true;
  }

  void prepend(ASTProgram *other) {
    funcs.insert(funcs.begin(), std::make_move_iterator(other->funcs.begin()),
                 std::make_move_iterator(other->funcs.end()));
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

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  [[nodiscard]] const ASTFuncDecl *get_function(const string &name) const {
    for (const auto &func : funcs) {
      if (func->name == name) {
        return func.get();
      }
    }
    throw runtime_error("Function not found");
  }

  std::string positionString() override {
    return "[MODULE]";
  }
};

#endif  // KL_AST_H
