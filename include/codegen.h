//
// Created by kiran on 5/21/24.
//

#ifndef KL_CODEGEN_H
#define KL_CODEGEN_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/ADT/StringRef.h"
#include <map>

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;

std::vector<std::map<std::string, llvm::Value*>> NamedValuesStack;

void enterScope() { NamedValuesStack.push_back({}); }

void exitScope() { NamedValuesStack.pop_back(); }

llvm::Value* findValue(const std::string& name) {
  for (auto it = NamedValuesStack.rbegin(); it != NamedValuesStack.rend();
       ++it) {
    auto& NamedValues = *it;
    auto found = NamedValues.find(name);
    if (found != NamedValues.end()) {
      return found->second;
    }
  }
  return nullptr;  // or throw an error
}

void addValue(const std::string& name, llvm::Value* value) {
  NamedValuesStack.back()[name] = value;
}

#endif //KL_CODEGEN_H
