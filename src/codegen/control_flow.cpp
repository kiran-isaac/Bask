#include "codegen.h"
#include <AST/AST.h>

using namespace llvm;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTControlFlowIf *node) {
  auto condition_result = node->condition->accept(this);
  if (condition_result->getTypeOfResult() == CodeGenResultType_Error) {
    return KLCodeGenResult::Error(condition_result->getError());
  }
  auto condition = condition_result->getValue();
  if (!condition)
    return KLCodeGenResult::Error("Failed to get value of condition");

  auto function = Builder.GetInsertBlock()->getParent();

  auto then_block = BasicBlock::Create(TheContext, "then", function);
  auto else_block = BasicBlock::Create(TheContext, "else");
  auto merge_block = BasicBlock::Create(TheContext, "ifcont");

  if (node->else_block) {
    Builder.CreateCondBr(condition, then_block, else_block);
  } else {
    Builder.CreateCondBr(condition, then_block, merge_block);
  }

  Builder.SetInsertPoint(then_block);
  node->then_block->accept(this);

  // check if the block is terminated
  if (!then_block->getTerminator()) 
    Builder.CreateBr(merge_block);

  then_block = Builder.GetInsertBlock();

  if (node->else_block) {
    function->getBasicBlockList().push_back(else_block);
    Builder.SetInsertPoint(else_block);

    node->else_block->accept(this);

    if (!else_block->getTerminator())
      Builder.CreateBr(merge_block);

    else_block = Builder.GetInsertBlock();

    // if both blocks are terminated, we don't need to create a merge block
    if (then_block->getTerminator() && else_block->getTerminator())
      return KLCodeGenResult::Halt();
  } 

  function->getBasicBlockList().push_back(merge_block);

  Builder.SetInsertPoint(merge_block);

  return KLCodeGenResult::NoHalt();
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTControlFlowWhile *node) {
  auto function = Builder.GetInsertBlock()->getParent();

  auto loop_block = BasicBlock::Create(TheContext, "loop", function);
  auto after_block = BasicBlock::Create(TheContext, "afterloop");

  Builder.CreateBr(loop_block);
  Builder.SetInsertPoint(loop_block);

  auto condition_result = node->condition->accept(this);
  auto condition = condition_result->getValue();
  if (!condition)
    return KLCodeGenResult::Error("Failed to get value of condition");

  Builder.CreateCondBr(condition, loop_block, after_block);

  function->getBasicBlockList().push_back(after_block);
  Builder.SetInsertPoint(after_block);

  return KLCodeGenResult::None();
}