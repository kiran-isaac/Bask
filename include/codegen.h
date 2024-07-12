//
// Created by kiran on 5/21/24.
//

#ifndef KL_CODEGEN_H
#define KL_CODEGEN_H

#include <map>

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
    return nullptr;  // or throw an error
  }

  void addValue(const std::string &name, llvm::Value *value) {
    NamedValuesStack.back()[name] = value;
  }
};

static NamedValuesClass NamedValues;

struct CodeGenResult {
  llvm::Value *value;
};

class KLCodeGenVisitor {
 private:
  llvm::LLVMContext TheContext;
  llvm::IRBuilder<> Builder;
  llvm::Module *TheModule;

 public:
  KLCodeGenVisitor(const char *module_name) : Builder(TheContext) {
    TheModule = new llvm::Module(module_name, TheContext);
  }

  // Declared in AST.h
  CodeGenResult visit(ASTType *node);
  CodeGenResult visit(ASTFuncDecl *node);
  CodeGenResult visit(ASTProgram *node);

  // Declared in AST_Expressions.h
  CodeGenResult visit(ASTExprConstantValue *node);
  CodeGenResult visit(ASTExprIdentifier *node);
  CodeGenResult visit(ASTExprFuncCall *node);
  CodeGenResult visit(ASTExprBinary *node);
  CodeGenResult visit(ASTExprUnary *node);

  // Declared in AST_Statements.h
  CodeGenResult visit(ASTStmtExpr *node);
  CodeGenResult visit(ASTStmtAssignment *node);
  CodeGenResult visit(ASTStmtDecl *node);
  CodeGenResult visit(ASTBlock *node);

  // Declared in AST_ControlFlow.h
  CodeGenResult visit(ASTControLFlowIf *node);
};

#endif  // KL_CODEGEN_H