#include "codegen.h"
#include <AST/AST.h>

using namespace llvm;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTControLFlowIf *node) {
  auto condition_result = node->condition->accept(this);
  auto condition = condition_result->getValue();
  if (!condition)
    return KLCodeGenResult::Error("Failed to get value of condition");

  auto function = Builder.GetInsertBlock()->getParent();

  auto then_block = BasicBlock::Create(TheContext, "then", function);
  auto else_block = BasicBlock::Create(TheContext, "else");
  auto merge_block = BasicBlock::Create(TheContext, "ifcont");

  Builder.CreateCondBr(condition, then_block, else_block);

  Builder.SetInsertPoint(then_block);
  node->then_block->accept(this);
  Builder.CreateBr(merge_block);
  then_block = Builder.GetInsertBlock();

  function->getBasicBlockList().push_back(else_block);
  Builder.SetInsertPoint(else_block);

  if (node->else_block) {
    node->else_block->accept(this);
  }

  Builder.CreateBr(merge_block);
  else_block = Builder.GetInsertBlock();

  function->getBasicBlockList().push_back(merge_block);
  Builder.SetInsertPoint(merge_block);

  return KLCodeGenResult::None();
}
