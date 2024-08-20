#include "codegen.h"
#include <AST/AST.h>
#include <llvm/IR/Constants.h>

using namespace llvm;

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTStmtExpr *node) {
  return node->expr->accept(this);
}

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTStmtAssignment *node) {
  auto var = NamedValues.findValue(node->identifier->name);
  if (!var)
    return BASKCodeGenResult::Error("Unknown variable name " + node->identifier->name);

  auto expr_result = node->value->accept(this);
  auto expr = expr_result->getValue();
  if (!expr)
    return BASKCodeGenResult::Error("Failed to get value of expression");

  Builder.CreateStore(expr, var);
  return BASKCodeGenResult::None();
}

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTStmtDecl *node) {
  auto type_result = node->type->accept(this);
  if (type_result->getTypeOfResult() != CodeGenResultType_Type)
    return BASKCodeGenResult::Error("Failed to get type of declaration " +
                                  node->identifier->name);
  
  auto type = type_result->getLLVMType();

  auto var = Builder.CreateAlloca(type, nullptr, node->identifier->name);

  if (node->value) {
    auto expr_result = node->value->accept(this);

    auto expr = expr_result->getValue();
    if (!expr)
      return BASKCodeGenResult::Error("Failed to get value of expression");

    NamedValues.addValue(node->identifier->name, expr);

    // create store using the type  
    Builder.CreateStore(expr, var);
  }

  return BASKCodeGenResult::Value(var);
}

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTBlock *node) {
  bool halt_next = false;
  for (auto &stmt : node->body) {
    if (halt_next) {
      return BASKCodeGenResult::Error("Unreachable code " + stmt->positionString());
    }
    auto result = stmt->accept(this);

    switch (result->getTypeOfResult()) {
      case CodeGenResultType_Error:
        return BASKCodeGenResult::Error(result->getError());
      case CodeGenResultType_Halt:
        // if this is the last statement in the block, return
        if (&stmt == &node->body.back())
          return BASKCodeGenResult::None();
        // Fail on the next stmt (so we can get correct line number)
        halt_next = true;
      case CodeGenResultType_NoHalt:
        // if this is the last statement in the block, return
        if (&stmt == &node->body.back())
          return BASKCodeGenResult::Error("Expected more statements after " + stmt->positionString() + " but found none. Possible missing return statement?");
        break;
      default:
        break;
    }
  }

  return BASKCodeGenResult::None();
}

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTStmtReturn *node) {
  auto expr_result = node->return_expr->accept(this);
  auto expr = expr_result->getValue();
  if (!expr)
    return BASKCodeGenResult::Error("Failed to get value of return expression");

  Builder.CreateRet(expr);
  return BASKCodeGenResult::Halt();
}