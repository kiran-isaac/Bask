#include "codegen.h"
#include <AST/AST.h>
#include <llvm/IR/Constants.h>

using namespace llvm;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTStmtExpr *node) {
  return node->expr->accept(this);
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTStmtAssignment *node) {}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTStmtDecl *node) {
  auto type_result = node->type->accept(this);
  if (type_result->getTypeOfResult() != CodeGenResultType_Type)
    return KLCodeGenResult::Error("Failed to get type of declaration " +
                                  node->identifier->name);
  
  auto type = type_result->getLLVMType();

  auto var = Builder.CreateAlloca(type, nullptr, node->identifier->name);
  NamedValues.addValue(node->identifier->name, var);

  if (node->value) {
    auto expr_result = node->value->accept(this);

    auto expr = expr_result->getValue();
    Builder.CreateStore(expr, var);
  }

  return KLCodeGenResult::Value(var);
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTBlock *node) {
  for (auto &stmt : node->body) {
    stmt->accept(this);
  }
  return KLCodeGenResult::None();
}