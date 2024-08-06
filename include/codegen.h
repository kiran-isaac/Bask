//
// Created by kiran on 5/21/24.
//

#ifndef KL_CODEGEN_H
#define KL_CODEGEN_H

#include <map>
#include <string>

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

class ASTType;
class ASTFuncDecl;
class ASTProgram;
class ASTExprConstantValue;
class ASTExprIdentifier;
class ASTExprFuncCall;
class ASTExprBinary;
class ASTExprUnary;
class ASTStmtExpr;
class ASTStmtAssignment;
class ASTStmtDecl;
class ASTBlock;
class ASTControLFlowIf;

class NamedValuesClass {
private:
  std::vector<std::map<std::string, llvm::Value *>> NamedValuesStack;

public:
  void enterScope() { NamedValuesStack.push_back({}); }

  void exitScope() { NamedValuesStack.pop_back(); }

  llvm::Value *findValue(const std::string &name) {
    for (auto it = NamedValuesStack.rbegin(); it != NamedValuesStack.rend();
         ++it) {
      auto &NamedValues = *it;
      auto found = NamedValues.find(name);
      if (found != NamedValues.end()) {
        return found->second;
      }
    }
    return nullptr; // or throw an error
  }

  void addValue(const std::string &name, llvm::Value *value) {
    NamedValuesStack.back()[name] = value;
  }
};

enum CodeGenResultType { CodeGenResultType_Value, CodeGenResultType_Error };

class KLCodeGenResult {
private:
  CodeGenResultType type;
  union {
    llvm::Value *value;
    std::string error;
  };

  KLCodeGenResult(CodeGenResultType type, llvm::Value *value, std::string error)
      : type(type) {
    switch (type) {
    case CodeGenResultType_Value:
      this->value = value;
      break;
    case CodeGenResultType_Error:
      this->error = error;
      break;
    }
  }

public:
  CodeGenResultType getTypeOfResult() { return type; }

  llvm::Value *getValue() {
    if (type != CodeGenResultType_Value) {
      throw std::runtime_error("Value is not available");
    }
    return value;
  }

  std::string getError() {
    if (type != CodeGenResultType_Error) {
      throw std::runtime_error("Error is not available");
    }
    return error;
  }

  static KLCodeGenResult *Error(const std::string &error) {
    return new KLCodeGenResult(CodeGenResultType_Error, nullptr, error);
  }

  static KLCodeGenResult *Value(llvm::Value *value) {
    return new KLCodeGenResult(CodeGenResultType_Value, value, "");
  }
};

class KLCodeGenVisitor {
private:
  llvm::LLVMContext TheContext;
  llvm::IRBuilder<> Builder;
  llvm::Module *TheModule;

public:
  NamedValuesClass NamedValues;

  KLCodeGenVisitor(const char *module_name) : Builder(TheContext) {
    TheModule = new llvm::Module(module_name, TheContext);
  }

  // Declared in AST.h
  KLCodeGenResult *visit(ASTType *node);
  KLCodeGenResult *visit(ASTFuncDecl *node);
  KLCodeGenResult *visit(ASTProgram *node);

  // Declared in AST_Expressions.h
  KLCodeGenResult *visit(ASTExprConstantValue *node);
  KLCodeGenResult *visit(ASTExprIdentifier *node);
  KLCodeGenResult *visit(ASTExprFuncCall *node);
  KLCodeGenResult *visit(ASTExprBinary *node);
  KLCodeGenResult *visit(ASTExprUnary *node);

  // Declared in AST_Statements.h
  KLCodeGenResult *visit(ASTStmtExpr *node);
  KLCodeGenResult *visit(ASTStmtAssignment *node);
  KLCodeGenResult *visit(ASTStmtDecl *node);
  KLCodeGenResult *visit(ASTBlock *node);

  // Declared in AST_ControlFlow.h
  KLCodeGenResult *visit(ASTControLFlowIf *node);
};

#endif // KL_CODEGEN_H