#include "AST/AST.h"
#include "codegen.h"
#include <llvm-14/llvm/Support/raw_os_ostream.h>
#include <llvm-14/llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Verifier.h>
#include <sstream>
#include <string>

using namespace llvm;
using namespace std;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTProgram *node) {
  auto result = KLCodeGenResult::None();
  for (auto &func : node->funcs) {
    auto func_result = func->accept(this);
    if (func_result->getTypeOfResult() != CodeGenResultType_Error)
      continue;
    
    if (result->getTypeOfResult() == CodeGenResultType_Error) {
      result->prepend_error(func_result->getError());
    } else {
      result = KLCodeGenResult::Error(func_result->getError());
    }
  }

  return result;
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTFuncDecl *node) {
  // Create the function prototype
  vector<Type *> argTypes;

  KLCodeGenResult *llvmTypeResult;
  for (auto &argType : node->argTypes) {
    llvmTypeResult = argType->accept(this);
    if (llvmTypeResult->getTypeOfResult() != CodeGenResultType_Type) {
      llvmTypeResult->prepend_error(
          "Function " + node->name + node->positionString() +
          " has bad argument: "); // Propagate the error
    }

    argTypes.push_back(llvmTypeResult->getLLVMType());
  }

  auto return_type = node->returnType->accept(this);

  if (return_type->getTypeOfResult() != CodeGenResultType_Type) {
    return_type->prepend_error("Function " + node->name +
                               node->positionString() +
                               " has bad return type: ");
    return return_type;
  }

  FunctionType *FT =
      FunctionType::get(return_type->getLLVMType(), argTypes, false);


  Function *F =
      Function::Create(FT, Function::ExternalLinkage, node->name, TheModule);

  // Set names for all arguments.
  unsigned idx = 0;
  unsigned argc = node->argNames.size();
  if (argc != argTypes.size()) {
    return KLCodeGenResult::Error("Function " + node->name +
                                  " has mismatched argument names and types");
  }
  for (auto &Arg : F->args()) {
    Arg.setName(node->argNames[idx++]);
  }

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.enterScope();
  idx = 0;
  for (auto &arg : F->args()) {
    NamedValues.addValue(node->argNames[idx++], &arg);
  }

  // Visit the function body
  auto body_result = node->body->accept(this);

  // Pop the function arguments from the NamedValues map.
  NamedValues.exitScope();

  auto func_result = KLCodeGenResult::None();

  if (body_result->getTypeOfResult() == CodeGenResultType_Error) {
    func_result = KLCodeGenResult::Error("Function " + node->name +
                                         node->positionString() +
                                         " has bad body: \n  " + body_result->getError() + "\n");
  }

  // create llvm os stream
  std::string error_msg;
  raw_string_ostream error_stream(error_msg);

  if (verifyFunction(*F, &error_stream)) {
    if (func_result->getTypeOfResult() == CodeGenResultType_None) {
      func_result = KLCodeGenResult::Error("");
    }

    func_result->prepend_error("Function " + node->name +
                                         node->positionString() +
                                         " failed llvm verification: \n  " + error_stream.str() + "\n");
  }

  return func_result;
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTType *node) {
  return KLCodeGenResult::Type(node->type.get_llvm_type(TheContext));
}