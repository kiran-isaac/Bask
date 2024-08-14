#include "AST/AST.h"
#include "codegen.h"
#include "tokens.h"
#include "types.h"
#include <llvm-14/llvm/Support/Debug.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>

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
    constant = ConstantInt::get(TheContext, APInt(64, string, 10));
    break;
  case KL_FLOAT_PRIMITIVE:
    constant = ConstantFP::get(llvm::Type::getDoubleTy(TheContext), string);
    break;
  case KL_BOOL_PRIMITIVE:
    constant =
        ConstantInt::get(TheContext, APInt(1, string == "true" ? "1" : "0", 2));
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

  // get type that the value is pointing to
  auto type = found->getType()->getPointerElementType();

  Value *loadedValue = Builder.CreateLoad(type, found, node->name);

  return KLCodeGenResult::Value(loadedValue);
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprFuncCall *node) {}

llvm::Value *KLCodeGenVisitor::i64_to_double(llvm::Value *value) {
  return Builder.CreateSIToFP(value, llvm::Type::getDoubleTy(TheContext),
                              "doubletmp");
}

llvm::Value *KLCodeGenVisitor::double_to_i64(llvm::Value *value) {
  return Builder.CreateFPToSI(value, llvm::Type::getInt64Ty(TheContext),
                              "inttmp");
}

llvm::Value *KLCodeGenVisitor::to_bool(llvm::Value *value) {
  if (value->getType() == llvm::Type::getInt1Ty(TheContext)) {
    return value;
  }
  if (value->getType() == llvm::Type::getInt64Ty(TheContext)) {
    return Builder.CreateICmpNE(value, ConstantInt::get(TheContext, APInt(64, 0)),
                                "booltmp");
  }
  if (value->getType() == llvm::Type::getInt8Ty(TheContext)) {
    return Builder.CreateICmpNE(value, ConstantInt::get(TheContext, APInt(8, 0)),
                                "booltmp");
  } else {
    throw std::runtime_error("UNREACHABLE: cannot convert to bool");
  }
}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTExprBinary *node) {
  auto lhs = node->lhs->accept(this);
  auto rhs = node->rhs->accept(this);

  if (lhs->getTypeOfResult() != CodeGenResultType_Value ||
      rhs->getTypeOfResult() != CodeGenResultType_Value) {
    return KLCodeGenResult::Error("Binary operation requires two values");
  }

  auto left = lhs->getValue();
  auto right = rhs->getValue();

  auto left_llvm_type = left->getType();
  auto right_llvm_type = right->getType();

  auto type = node->get_expr_type();

  auto lhs_primitive = node->lhs->get_expr_type().primitive;
  auto rhs_primitive = node->rhs->get_expr_type().primitive;

  switch (node->op) {
  // Arithmetic binary operators: +, -, *, /, %
  case ARITHMETIC_BINARY_CASES:
    // If one of the operands is an integer and the other is a float, convert
    // the integer to a float
    if (lhs_primitive == KL_INT_PRIMITIVE &&
        rhs_primitive == KL_FLOAT_PRIMITIVE) {
      left = i64_to_double(left);
    } else if (lhs_primitive == KL_FLOAT_PRIMITIVE &&
               rhs_primitive == KL_INT_PRIMITIVE) {
      right = i64_to_double(right);
    }

    switch (type.primitive) {
    case KL_INT_PRIMITIVE:
      switch (node->op) {
      case KL_TT_Operator_Add:
        return KLCodeGenResult::Value(Builder.CreateAdd(left, right, "addtmp"));
      case KL_TT_Operator_Sub:
        return KLCodeGenResult::Value(Builder.CreateSub(left, right, "subtmp"));
      case KL_TT_Operator_Mul:
        return KLCodeGenResult::Value(Builder.CreateMul(left, right, "multmp"));
      case KL_TT_Operator_Div:
        return KLCodeGenResult::Value(
            Builder.CreateSDiv(left, right, "divtmp"));
      case KL_TT_Operator_Mod:
        return KLCodeGenResult::Value(
            Builder.CreateSRem(left, right, "modtmp"));
      default:
        return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
      }
      break;
    case KL_FLOAT_PRIMITIVE:
      switch (node->op) {
      case KL_TT_Operator_Add:
        return KLCodeGenResult::Value(
            Builder.CreateFAdd(left, right, "addtmp"));
      case KL_TT_Operator_Sub:
        return KLCodeGenResult::Value(
            Builder.CreateFSub(left, right, "subtmp"));
      case KL_TT_Operator_Mul:
        return KLCodeGenResult::Value(
            Builder.CreateFMul(left, right, "multmp"));
      case KL_TT_Operator_Div:
        return KLCodeGenResult::Value(
            Builder.CreateFDiv(left, right, "divtmp"));
      case KL_TT_Operator_Mod:
        return KLCodeGenResult::Value(
            Builder.CreateFRem(left, right, "modtmp"));
      default:
        return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
      }
      break;
    default:
      return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
    }
  case LOGICAL_BINARY_CASES:
    if (lhs_primitive != KL_BOOL_PRIMITIVE && lhs_primitive != KL_INT_PRIMITIVE &&
        lhs_primitive != KL_CHAR_PRIMITIVE && rhs_primitive != KL_BOOL_PRIMITIVE &&
        rhs_primitive != KL_INT_PRIMITIVE && rhs_primitive != KL_CHAR_PRIMITIVE) {
      return KLCodeGenResult::Error("UNREACHABLE: Invalid type for logical binary operator");
    }

    left = to_bool(left);
    right = to_bool(right);

    switch (node->op) {
    case KL_TT_Operator_LogicalAnd:
      return KLCodeGenResult::Value(
          Builder.CreateAnd(left, right, "andtmp"));
    case KL_TT_Operator_LogicalOr:
      return KLCodeGenResult::Value(
          Builder.CreateOr(left, right, "ortmp"));
    default:
      return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
    }
  // Comparison operators: ==, !=, <, <=, >, >=
  case COMPARISON_BINARY_CASES:
    // If one of the operands is an integer and the other is a float, convert
    // the integer to a float
    if (lhs_primitive == KL_INT_PRIMITIVE &&
        rhs_primitive == KL_FLOAT_PRIMITIVE) {
      left = i64_to_double(left);
    } else if (lhs_primitive == KL_FLOAT_PRIMITIVE &&
               rhs_primitive == KL_INT_PRIMITIVE) {
      right = i64_to_double(right);
    }

    switch (lhs_primitive) {
      // Compare as ints
      case KL_INT_PRIMITIVE:
        switch (node->op) {
        case KL_TT_Operator_Equal:
          return KLCodeGenResult::Value(
              Builder.CreateICmpEQ(left, right, "eqtmp"));
        case KL_TT_Operator_NotEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpNE(left, right, "netmp"));
        case KL_TT_Operator_Less:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSLT(left, right, "lttmp"));
        case KL_TT_Operator_LessEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSLE(left, right, "letmp"));
        case KL_TT_Operator_Greater:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSGT(left, right, "gttmp"));
        case KL_TT_Operator_GreaterEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSGE(left, right, "getmp"));
        default:
          return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as floats
      case KL_FLOAT_PRIMITIVE:
        switch (node->op) {
        case KL_TT_Operator_Equal:
          return KLCodeGenResult::Value(
              Builder.CreateFCmpOEQ(left, right, "eqtmp"));
        case KL_TT_Operator_NotEqual:
          return KLCodeGenResult::Value(
              Builder.CreateFCmpONE(left, right, "netmp"));
        case KL_TT_Operator_Less:
          return KLCodeGenResult::Value(
              Builder.CreateFCmpOLT(left, right, "lttmp"));
        case KL_TT_Operator_LessEqual:
          return KLCodeGenResult::Value(
              Builder.CreateFCmpOLE(left, right, "letmp"));
        case KL_TT_Operator_Greater:
          return KLCodeGenResult::Value(
              Builder.CreateFCmpOGT(left, right, "gttmp"));
        case KL_TT_Operator_GreaterEqual:
          return KLCodeGenResult::Value(
              Builder.CreateFCmpOGE(left, right, "getmp"));
        default:
          return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as bools
      case KL_BOOL_PRIMITIVE:

        switch (node->op) {
        case KL_TT_Operator_Equal:
          return KLCodeGenResult::Value(
              Builder.CreateICmpEQ(left, right, "eqtmp"));
        case KL_TT_Operator_NotEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpNE(left, right, "netmp"));
        default:
          return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as chars
      case KL_CHAR_PRIMITIVE:
        switch (node->op) {
        case KL_TT_Operator_Equal:
          return KLCodeGenResult::Value(
              Builder.CreateICmpEQ(left, right, "eqtmp"));
        case KL_TT_Operator_NotEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpNE(left, right, "netmp"));
        case KL_TT_Operator_Less:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSLT(left, right, "lttmp"));
        case KL_TT_Operator_LessEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSLE(left, right, "letmp"));
        case KL_TT_Operator_Greater:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSGT(left, right, "gttmp"));
        case KL_TT_Operator_GreaterEqual:
          return KLCodeGenResult::Value(
              Builder.CreateICmpSGE(left, right, "getmp"));
        default:
          return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as strings
      case KL_STRING_PRIMITIVE:
        // TODO: Implement string comparison
        return KLCodeGenResult::Error("UNIMPLEMENTED: STRING COMPARISON");
      default:
        return KLCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN (VOID?????)");
    }
  default:
    return KLCodeGenResult::Error("Unknown binary operator: " +
                                  std::to_string(node->op));
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