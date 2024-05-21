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

using namespace llvm;

class CodeGen {
private:
  LLVMContext context;
public:
  explicit CodeGen(const std::string& filename);
  
  void generate();
};

#endif //KL_CODEGEN_H
