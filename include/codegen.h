//
// Created by kiran on 5/21/24.
//

#ifndef KL_CODEGEN_H
#define KL_CODEGEN_H

#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <string>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

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
class ASTStmtReturn;
class ASTBlock;
class ASTControlFlowIf;
class ASTControlFlowWhile;

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

enum CodeGenResultType {
  CodeGenResultType_Value,
  CodeGenResultType_Error,
  CodeGenResultType_None,
  CodeGenResultType_Halt,
  CodeGenResultType_Type
};

class KLCodeGenResult {
private:
  CodeGenResultType type;

  llvm::Value *value;
  llvm::Type *llvm_type;
  std::string error;
  

  KLCodeGenResult(CodeGenResultType type, std::optional<llvm::Value *> value,
                  std::optional<std::string> error,
                  std::optional<llvm::Type *> llvm_type)
      : type(type) {
    switch (type) {
    case CodeGenResultType_Value:
      if (!value.has_value()) {
        throw std::runtime_error("Value must be provided");
      }
      this->value = value.value();
      break;
    case CodeGenResultType_Error:
      if (!error.has_value()) {
        throw std::runtime_error("Error must be provided");
      }
      this->error = error.value();
      break;
    case CodeGenResultType_Type:
      if (!llvm_type.has_value()) {
        throw std::runtime_error("Type must be provided");
      }
      this->llvm_type = llvm_type.value();
      break;
    case CodeGenResultType_Halt:
    case CodeGenResultType_None:
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

  llvm::Type *getLLVMType() {
    if (type != CodeGenResultType_Type) {
      throw std::runtime_error("Type is not available");
    }
    return llvm_type;
  }

  void prepend_error(const std::string &error) {
    if (type != CodeGenResultType_Error) {
      throw std::runtime_error("Error is not available");
    }
    this->error = error + this->error;
  }

  void append_error(const std::string &error) {
    if (type != CodeGenResultType_Error) {
      throw std::runtime_error("Error is not available");
    }
    this->error += error;
  }

  static KLCodeGenResult *Error(std::string error) {
    return new KLCodeGenResult(CodeGenResultType_Error, std::nullopt, std::make_optional(error),
                               std::nullopt);
  }

  static KLCodeGenResult *Value(llvm::Value *value) {
    return new KLCodeGenResult(CodeGenResultType_Value, value, "", nullptr);
  }

  static KLCodeGenResult *Type(llvm::Type *llvm_type) {
    return new KLCodeGenResult(CodeGenResultType_Type, nullptr, "", llvm_type);
  }

  static KLCodeGenResult *Halt() {
    return new KLCodeGenResult(CodeGenResultType_Halt, nullptr, "", nullptr);
  }

  static KLCodeGenResult *None() {
    return new KLCodeGenResult(CodeGenResultType_None, nullptr, "", nullptr);
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

    TheModule->setDataLayout("e-m:e-i64:64-f80:128-n8:16:32:64-S128");
  }

  llvm::Value *i64_to_double(llvm::Value *value);
  llvm::Value *double_to_i64(llvm::Value *value);

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
  KLCodeGenResult *visit(ASTControlFlowIf *node);
  KLCodeGenResult *visit(ASTControlFlowWhile *node);
  KLCodeGenResult *visit(ASTStmtReturn *node);

  llvm::Module *getModule() { return TheModule; }

  void printModule(llvm::raw_ostream &stream = llvm::outs()) {
    TheModule->print(stream, nullptr);
  }

  std::string getModuleAsString() {
    std::string output;
    llvm::raw_string_ostream llvm_output(output);
    printModule(llvm_output);
    return output;
  }
};

#endif // KL_CODEGEN_H