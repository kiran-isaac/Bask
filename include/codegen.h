//
// Created by kiran on 5/21/24.
//

#ifndef BASK_CODEGEN_H
#define BASK_CODEGEN_H

#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <fstream>
#include <unistd.h>

#include <options.h>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <optional>

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

  void addValue(const std::string name, llvm::Value *value) {
    NamedValuesStack.back()[name] = value;
  }
};

enum CodeGenResultType {
  CodeGenResultType_Value,
  CodeGenResultType_Error,
  CodeGenResultType_None,
  CodeGenResultType_Halt,   // make sure this is the last statement in the block
  CodeGenResultType_NoHalt, // make sure this is not the last statement in the block
  CodeGenResultType_Type,

};

class BASKCodeGenResult {
private:
  CodeGenResultType type;

  llvm::Value *value;
  llvm::Type *llvm_type;
  std::string error;
  

  BASKCodeGenResult(CodeGenResultType type, std::optional<llvm::Value *> value,
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
    case CodeGenResultType_NoHalt:
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

  static BASKCodeGenResult *Error(std::string error) {
    return new BASKCodeGenResult(CodeGenResultType_Error, std::nullopt, std::make_optional(error),
                               std::nullopt);
  }

  static BASKCodeGenResult *Value(llvm::Value *value) {
    return new BASKCodeGenResult(CodeGenResultType_Value, value, "", nullptr);
  }

  static BASKCodeGenResult *Type(llvm::Type *llvm_type) {
    return new BASKCodeGenResult(CodeGenResultType_Type, nullptr, "", llvm_type);
  }

  static BASKCodeGenResult *NoHalt() {
    return new BASKCodeGenResult(CodeGenResultType_NoHalt, nullptr, "", nullptr);
  }

  static BASKCodeGenResult *Halt() {
    return new BASKCodeGenResult(CodeGenResultType_Halt, nullptr, "", nullptr);
  }

  static BASKCodeGenResult *None() {
    return new BASKCodeGenResult(CodeGenResultType_None, nullptr, "", nullptr);
  }
};

class BASKCodeGenVisitor {
private:
  llvm::LLVMContext TheContext;
  llvm::IRBuilder<> Builder;
  llvm::Module *TheModule;

public:
  NamedValuesClass NamedValues;

  BASKCodeGenVisitor(const char *module_name) : Builder(TheContext) {
    TheModule = new llvm::Module(module_name, TheContext);

    // Set target triple
    TheModule->setTargetTriple(llvm::sys::getDefaultTargetTriple());
  }

  llvm::Value *i64_to_double(llvm::Value *value);
  llvm::Value *double_to_i64(llvm::Value *value);
  llvm::Value *to_bool(llvm::Value *value);

  // Declared in AST.h
  BASKCodeGenResult *visit(ASTType *node);
  BASKCodeGenResult *visit(ASTFuncDecl *node);
  BASKCodeGenResult *visit(ASTProgram *node);

  // Declared in AST_Expressions.h
  BASKCodeGenResult *visit(ASTExprConstantValue *node);
  BASKCodeGenResult *visit(ASTExprIdentifier *node);
  BASKCodeGenResult *visit(ASTExprFuncCall *node);
  BASKCodeGenResult *visit(ASTExprBinary *node);
  BASKCodeGenResult *visit(ASTExprUnary *node);

  // Declared in AST_Statements.h
  BASKCodeGenResult *visit(ASTStmtExpr *node);
  BASKCodeGenResult *visit(ASTStmtAssignment *node);
  BASKCodeGenResult *visit(ASTStmtDecl *node);
  BASKCodeGenResult *visit(ASTBlock *node);

  // Declared in AST_ControlFlow.h
  BASKCodeGenResult *visit(ASTControlFlowIf *node);
  BASKCodeGenResult *visit(ASTControlFlowWhile *node);
  BASKCodeGenResult *visit(ASTStmtReturn *node);

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

  std::string getModuleOptimizedAsString(std::string optimization_level) {
    if (optimization_level == "0") {
      return getModuleAsString();
    }
    string module = getModuleAsString();
    system("mkdir -p /tmp/bask");
    string filename = "/tmp/bask/unoptimised.ll";
    ofstream out(filename);
    out << module;
    out.close();
    string command = "opt -S " + filename + " -o /tmp/bask/optimised.ll 2> /tmp/bask/opt.log -O" + optimization_level;
    if (system(command.c_str()) != 0) {
      cerr << "Error: failed to optimize" << endl;
      cerr << "Opt invocation: " << command << endl;
      ifstream log("/tmp/bask/opt.log");
      string line;
      while (getline(log, line)) {
        cerr << line << endl;
      }
      cerr << "Opt log dumped to /tmp/bask/opt.log" << endl;
      exit(1);
    }
    ifstream in("/tmp/bask/optimised.ll");
    string optimized_module((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return optimized_module;
  }

  void compileModule(CommandLineArguments &options);
};

#endif // BASK_CODEGEN_H