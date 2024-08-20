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
BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTExprConstantValue *node) {
  auto type = node->type;

  if (!type.is_const) {
    return BASKCodeGenResult::Error("Constant value is not constant huh???");
  }

  auto string = llvm::StringRef(node->value);
  Value *constant;

  switch (type.primitive) {
  case BASK_INT_PRIMITIVE:
    constant = ConstantInt::get(TheContext, APInt(64, string, 10));
    break;
  case BASK_FLOAT_PRIMITIVE:
    constant = ConstantFP::get(llvm::Type::getDoubleTy(TheContext), string);
    break;
  case BASK_BOOL_PRIMITIVE:
    constant =
        ConstantInt::get(TheContext, APInt(1, string == "true" ? "1" : "0", 2));
    break;
  case BASK_CHAR_PRIMITIVE:
    constant = ConstantInt::get(TheContext, APInt(8, node->value[0], 10));
    break;
  case BASK_STRING_PRIMITIVE:
    constant = ConstantDataArray::getString(TheContext, node->value);
    break;
  default:
    return BASKCodeGenResult::Error("Unknown type");
  }

  return BASKCodeGenResult::Value(constant);
}

// Get the value of an identifier, create a load instruction and return the
// value
BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTExprIdentifier *node) {
  auto found = NamedValues.findValue(node->name);

  if (found == nullptr) {
    return BASKCodeGenResult::Error("Unknown variable name");
  }

  // get type that the value is pointing to
  auto type = found->getType();

  return BASKCodeGenResult::Value(found);
}

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTExprFuncCall *node) {
  auto func = TheModule->getFunction(node->name);

  if (!func) {
    return BASKCodeGenResult::Error("Unknown function referenced");
  }

  if (func->arg_size() != node->args->size()) {
    return BASKCodeGenResult::Error("Incorrect number of arguments passed");
  }

  auto function_args = func->args().begin();
  auto call_args = node->args->begin();

  for (; function_args != func->args().end(); ++function_args, ++call_args) {
    auto *arg = call_args->get();
    auto arg_type = (*function_args).getType();
    auto arg_result = arg->accept(this);

    if (arg_result->getTypeOfResult() != CodeGenResultType_Value) {
      return BASKCodeGenResult::Error("Function call requires values");
    }

    auto arg_value = arg_result->getValue();
    auto arg_value_type = arg_value->getType();

    if (arg_value_type != arg_type) {
      return BASKCodeGenResult::Error("Argument type mismatch");
    }
  }

  std::vector<Value *> args;
  for (auto &arg : *node->args) {
    auto arg_result = arg->accept(this);
    if (arg_result->getTypeOfResult() != CodeGenResultType_Value) {
      return BASKCodeGenResult::Error("Function call requires values");
    }

    args.push_back(arg_result->getValue());
  }

  return BASKCodeGenResult::Value(Builder.CreateCall(func, args, "calltmp"));
}

llvm::Value *BASKCodeGenVisitor::i64_to_double(llvm::Value *value) {
  return Builder.CreateSIToFP(value, llvm::Type::getDoubleTy(TheContext),
                              "doubletmp");
}

llvm::Value *BASKCodeGenVisitor::double_to_i64(llvm::Value *value) {
  return Builder.CreateFPToSI(value, llvm::Type::getInt64Ty(TheContext),
                              "inttmp");
}

llvm::Value *BASKCodeGenVisitor::to_bool(llvm::Value *value) {
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

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTExprBinary *node) {
  auto lhs = node->lhs->accept(this);
  auto rhs = node->rhs->accept(this);

  if (lhs->getTypeOfResult() != CodeGenResultType_Value ||
      rhs->getTypeOfResult() != CodeGenResultType_Value) {
    return BASKCodeGenResult::Error("Binary operation requires two values");
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
    if (lhs_primitive == BASK_INT_PRIMITIVE &&
        rhs_primitive == BASK_FLOAT_PRIMITIVE) {
      left = i64_to_double(left);
    } else if (lhs_primitive == BASK_FLOAT_PRIMITIVE &&
               rhs_primitive == BASK_INT_PRIMITIVE) {
      right = i64_to_double(right);
    }

    switch (type.primitive) {
    case BASK_INT_PRIMITIVE:
      switch (node->op) {
      case BASK_TT_Operator_Add:
        return BASKCodeGenResult::Value(Builder.CreateAdd(left, right, "addtmp"));
      case BASK_TT_Operator_Sub:
        return BASKCodeGenResult::Value(Builder.CreateSub(left, right, "subtmp"));
      case BASK_TT_Operator_Mul:
        return BASKCodeGenResult::Value(Builder.CreateMul(left, right, "multmp"));
      case BASK_TT_Operator_Div:
        return BASKCodeGenResult::Value(
            Builder.CreateSDiv(left, right, "divtmp"));
      case BASK_TT_Operator_Mod:
        return BASKCodeGenResult::Value(
            Builder.CreateSRem(left, right, "modtmp"));
      default:
        return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
      }
      break;
    case BASK_FLOAT_PRIMITIVE:
      switch (node->op) {
      case BASK_TT_Operator_Add:
        return BASKCodeGenResult::Value(
            Builder.CreateFAdd(left, right, "addtmp"));
      case BASK_TT_Operator_Sub:
        return BASKCodeGenResult::Value(
            Builder.CreateFSub(left, right, "subtmp"));
      case BASK_TT_Operator_Mul:
        return BASKCodeGenResult::Value(
            Builder.CreateFMul(left, right, "multmp"));
      case BASK_TT_Operator_Div:
        return BASKCodeGenResult::Value(
            Builder.CreateFDiv(left, right, "divtmp"));
      case BASK_TT_Operator_Mod:
        return BASKCodeGenResult::Value(
            Builder.CreateFRem(left, right, "modtmp"));
      default:
        return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
      }
      break;
    default:
      return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
    }
  case LOGICAL_BINARY_CASES:
    if (lhs_primitive != BASK_BOOL_PRIMITIVE && lhs_primitive != BASK_INT_PRIMITIVE &&
        lhs_primitive != BASK_CHAR_PRIMITIVE && rhs_primitive != BASK_BOOL_PRIMITIVE &&
        rhs_primitive != BASK_INT_PRIMITIVE && rhs_primitive != BASK_CHAR_PRIMITIVE) {
      return BASKCodeGenResult::Error("UNREACHABLE: Invalid type for logical binary operator");
    }

    left = to_bool(left);
    right = to_bool(right);

    switch (node->op) {
    case BASK_TT_Operator_LogicalAnd:
      return BASKCodeGenResult::Value(
          Builder.CreateAnd(left, right, "andtmp"));
    case BASK_TT_Operator_LogicalOr:
      return BASKCodeGenResult::Value(
          Builder.CreateOr(left, right, "ortmp"));
    default:
      return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
    }
  // Comparison operators: ==, !=, <, <=, >, >=
  case COMPARISON_BINARY_CASES:
    // If one of the operands is an integer and the other is a float, convert
    // the integer to a float
    if (lhs_primitive == BASK_INT_PRIMITIVE &&
        rhs_primitive == BASK_FLOAT_PRIMITIVE) {
      left = i64_to_double(left);
    } else if (lhs_primitive == BASK_FLOAT_PRIMITIVE &&
               rhs_primitive == BASK_INT_PRIMITIVE) {
      right = i64_to_double(right);
    }

    switch (lhs_primitive) {
      // Compare as ints
      case BASK_INT_PRIMITIVE:
        switch (node->op) {
        case BASK_TT_Operator_Equal:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpEQ(left, right, "eqtmp"));
        case BASK_TT_Operator_NotEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpNE(left, right, "netmp"));
        case BASK_TT_Operator_Less:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSLT(left, right, "lttmp"));
        case BASK_TT_Operator_LessEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSLE(left, right, "letmp"));
        case BASK_TT_Operator_Greater:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSGT(left, right, "gttmp"));
        case BASK_TT_Operator_GreaterEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSGE(left, right, "getmp"));
        default:
          return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as floats
      case BASK_FLOAT_PRIMITIVE:
        switch (node->op) {
        case BASK_TT_Operator_Equal:
          return BASKCodeGenResult::Value(
              Builder.CreateFCmpOEQ(left, right, "eqtmp"));
        case BASK_TT_Operator_NotEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateFCmpONE(left, right, "netmp"));
        case BASK_TT_Operator_Less:
          return BASKCodeGenResult::Value(
              Builder.CreateFCmpOLT(left, right, "lttmp"));
        case BASK_TT_Operator_LessEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateFCmpOLE(left, right, "letmp"));
        case BASK_TT_Operator_Greater:
          return BASKCodeGenResult::Value(
              Builder.CreateFCmpOGT(left, right, "gttmp"));
        case BASK_TT_Operator_GreaterEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateFCmpOGE(left, right, "getmp"));
        default:
          return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as bools
      case BASK_BOOL_PRIMITIVE:

        switch (node->op) {
        case BASK_TT_Operator_Equal:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpEQ(left, right, "eqtmp"));
        case BASK_TT_Operator_NotEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpNE(left, right, "netmp"));
        default:
          return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as chars
      case BASK_CHAR_PRIMITIVE:
        switch (node->op) {
        case BASK_TT_Operator_Equal:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpEQ(left, right, "eqtmp"));
        case BASK_TT_Operator_NotEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpNE(left, right, "netmp"));
        case BASK_TT_Operator_Less:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSLT(left, right, "lttmp"));
        case BASK_TT_Operator_LessEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSLE(left, right, "letmp"));
        case BASK_TT_Operator_Greater:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSGT(left, right, "gttmp"));
        case BASK_TT_Operator_GreaterEqual:
          return BASKCodeGenResult::Value(
              Builder.CreateICmpSGE(left, right, "getmp"));
        default:
          return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN");
        }
      // Compare as strings
      case BASK_STRING_PRIMITIVE:
        // TODO: Implement string comparison
        return BASKCodeGenResult::Error("UNIMPLEMENTED: STRING COMPARISON");
      default:
        return BASKCodeGenResult::Error("UNREACHABLE: BINARY CODEGEN (VOID?????)");
    }
  default:
    return BASKCodeGenResult::Error("Unknown binary operator: " +
                                  std::to_string(node->op));
  }
}

BASKCodeGenResult *BASKCodeGenVisitor::visit(ASTExprUnary *node) {
  auto operand = node->expr->accept(this);

  if (operand->getTypeOfResult() != CodeGenResultType_Value) {
    return BASKCodeGenResult::Error("Unary operation requires a value");
  }

  auto value = operand->getValue();

  switch (node->op) {
  case BASK_TT_Operator_Sub:
    return BASKCodeGenResult::Value(Builder.CreateNeg(value, "negtmp"));
  case BASK_TT_Operator_LogicalNot:
  case BASK_TT_Operator_BitwiseNot:
    return BASKCodeGenResult::Value(Builder.CreateNot(value, "nottmp"));
  default:
    return BASKCodeGenResult::Error("Unknown unary operator");
  }
}