#include "AST/AST.h"
#include "codegen.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTProgram *node) {
  for (auto &func : node->funcs) {
    func->accept(this);
  }
  return KLCodeGenResult::None();
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTFuncDecl *node) {
  // Create the function prototype
  vector<Type *> argTypes;
  for (auto &argType : node->argTypes) {
    argTypes.push_back(argType->to_llvm_type());
  }
  
  return KLCodeGenResult::None();
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTType *node) {}