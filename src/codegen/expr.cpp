#include "codegen.h"
#include "llvm/IR/Constants.h"
#include <llvm-14/llvm/ADT/StringRef.h>
#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/InstrTypes.h>
#include <llvm-14/llvm/IR/Type.h>
#include <llvm-14/llvm/IR/Value.h>
#include "AST/AST.h"
#include "types.h"

using namespace llvm;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprConstantValue *node) {
  auto type = node->type;

  if (!type.is_const) {
    return KLCodeGenResult::Error("Constant value is not constant huh???");
  }

  auto string = llvm::StringRef(node->value);

  switch (type.primitive) {
    case KL_INT:
      return KLCodeGenResult::Value(
          ConstantInt::get(TheContext, APInt(32, string, 10)));

    case KL_FLOAT:
      return KLCodeGenResult::Value(
          ConstantFP::get(TheContext, APFloat(APFloat::IEEEsingle(), string)));

    case KL_BOOL:
      return KLCodeGenResult::Value(
              ConstantInt::get(TheContext, APInt(1, string == "true" ? "1" : "0", 2)));

    case KL_CHAR:
      return KLCodeGenResult::Value(
          ConstantInt::get(TheContext, APInt(8, node->value[0], 10)));

    case KL_STRING :
        return KLCodeGenResult::Value(
          ConstantDataArray::getString(TheContext, node->value));

    default:
      return KLCodeGenResult::Error("Unknown type");
  }
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprIdentifier *node) {
  auto found = NamedValues.findValue(node->name);

  if (found == nullptr) {
    return KLCodeGenResult::Error("Unknown variable name");
  }
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprFuncCall *node) {}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprBinary *node) {
  auto lhs = node->lhs->accept(this);
  auto rhs = node->rhs->accept(this);

  if (lhs->getTypeOfResult() != CodeGenResultType_Value || rhs->getTypeOfResult() != CodeGenResultType_Value) {
    return KLCodeGenResult::Error("Binary operation requires two values");
  }
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprUnary *node) {}