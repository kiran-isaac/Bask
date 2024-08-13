#include "AST/AST.h"
#include "codegen.h"
#include "tokens.h"
#include "types.h"
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

using namespace llvm;

// Get the value of a constant, create a constant value and return it
KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprConstantValue *node) {
  auto type = node->type;

  if (!type.is_const) {
    return KLCodeGenResult::Error("Constant value is not constant huh???");
  }

  auto string = llvm::StringRef(node->value);
  Value *constant;

  switch (type.primitive) {
  case KL_INT_PRIMITIVE:
    constant = ConstantInt::get(TheContext, APInt(32, string, 10));
    break;
  case KL_FLOAT_PRIMITIVE:
    constant =
        ConstantFP::get(TheContext, APFloat(APFloat::IEEEsingle(), string));
    break;
  case KL_BOOL_PRIMITIVE:
    constant = ConstantInt::get(TheContext, APInt(1, string == "true" ? "1" : "0", 2));
    break;
  case KL_CHAR_PRIMITIVE:
    constant = ConstantInt::get(TheContext, APInt(8, node->value[0], 10));
    break;
  case KL_STRING_PRIMITIVE:
    constant = ConstantDataArray::getString(TheContext, node->value);
    break;
  default:
    return KLCodeGenResult::Error("Unknown type");
  }

  return KLCodeGenResult::Value(constant);
}

// Get the value of an identifier, create a load instruction and return the
// value
KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprIdentifier *node) {
  auto found = NamedValues.findValue(node->name);

  if (found == nullptr) {
    return KLCodeGenResult::Error("Unknown variable name");
  }

  Value *loadedValue = Builder.CreateLoad(
      found->getType()->getPointerElementType(), found, node->name);

  return KLCodeGenResult::Value(loadedValue);
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprFuncCall *node) {}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprBinary *node) {
  auto lhs = node->lhs->accept(this);
  auto rhs = node->rhs->accept(this);

  if (lhs->getTypeOfResult() != CodeGenResultType_Value ||
      rhs->getTypeOfResult() != CodeGenResultType_Value) {
    return KLCodeGenResult::Error("Binary operation requires two values");
  }
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprUnary *node) {
  auto operand = node->expr->accept(this);

  if (operand->getTypeOfResult() != CodeGenResultType_Value) {
    return KLCodeGenResult::Error("Unary operation requires a value");
  }

  auto value = operand->getValue();

  switch (node->op) {
  case KL_TT_Operator_Sub:
    return KLCodeGenResult::Value(Builder.CreateNeg(value, "negtmp"));
  case KL_TT_Operator_LogicalNot:
  case KL_TT_Operator_BitwiseNot:
    return KLCodeGenResult::Value(Builder.CreateNot(value, "nottmp"));
  default:
    return KLCodeGenResult::Error("Unknown unary operator");
  }
}