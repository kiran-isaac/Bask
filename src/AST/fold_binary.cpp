//
// Created by kiran on 5/21/24.
//

#include <cmath>

#include "AST/AST.h"

unique_ptr<ASTExpr> ASTExpr::fold_binary(ASTExpr *expr) {
  auto binary = dynamic_cast<ASTExprBinary *>(expr);
  binary->lhs = fold(binary->lhs.get());
  binary->rhs = fold(binary->rhs.get());

  // Optimisation: Extract as many constants as possible. At the moment
  // expressions aren't re-arranged if the operator is commutative
  if (binary->lhs->get_AST_type() != ASTNode::ASTNodeType::ExprConstValue ||
      binary->rhs->get_AST_type() != ASTNode::ASTNodeType::ExprConstValue) {
    return make_unique<ASTExprBinary>(std::move(binary->lhs),
                                      std::move(binary->rhs), binary->op,
                                      binary->line, binary->col);
  }

  binary->check_semantics();

  auto lhs = dynamic_cast<ASTExprConstantValue *>(binary->lhs.get());
  auto rhs = dynamic_cast<ASTExprConstantValue *>(binary->rhs.get());

  if (!lhs || !rhs || lhs->type.kind != KL_PRIMITIVE_TYPEKIND ||
      rhs->type.kind != KL_PRIMITIVE_TYPEKIND) {
    throw std::runtime_error("Binary expression does not have constant values");
  }

  // If either is a string, then the other must be a string or a char
  if (lhs->type.primitive == KL_STRING_PRIMITIVE || rhs->type.primitive == KL_STRING_PRIMITIVE) {
    // v1 is definitely a string
    auto v1 = lhs->type.primitive == KL_STRING_PRIMITIVE ? lhs : rhs;
    string s1 = v1->value;

    // v2 could be a string, a char
    auto v2 = lhs->type.primitive == KL_STRING_PRIMITIVE ? rhs : lhs;
    string s2;

    switch (v2->type.primitive) {
      default:
        ASTNode::SyntaxError(binary->line, binary->col, "Invalid type in string expression");
      case KL_STRING_PRIMITIVE:
      case KL_CHAR_PRIMITIVE:
        s2 = v2->value;
    }

    switch (binary->op) {
      case KL_TT_Operator_Add:
        return make_unique<ASTExprConstantValue>(v1->type, s1 + s2, v1->line,
                                                 v1->col);
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, s1 == s2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, s1 != s2 ? "true" : "false", v1->line, v1->col);
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid operator in string expression after "
            "semantic check");
    }
  // If either is a float, then the other must be a float or an int
  } else if (lhs->type.primitive == KL_FLOAT_PRIMITIVE ||
             rhs->type.primitive == KL_FLOAT_PRIMITIVE) {
    // v1 is definitely a float
    auto v1 = lhs->type.primitive == KL_FLOAT_PRIMITIVE ? lhs : rhs;

    // v2 could be a float, an int
    auto v2 = lhs->type.primitive == KL_FLOAT_PRIMITIVE ? rhs : lhs;

    float f1 = stof(v1->value);

    float f2;

    switch (v2->type.primitive) {
      case KL_FLOAT_PRIMITIVE:
      case KL_INT_PRIMITIVE:
        f2 = stof(v2->value);
        break;
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid type in float expression after semantic "
            "check");
    }

    switch (binary->op) {
      case KL_TT_Operator_Add:
        return make_unique<ASTExprConstantValue>(
            KL_FLOAT_CONST, to_string(f1 + f2), v1->line, v1->col);
      case KL_TT_Operator_Sub:
        return make_unique<ASTExprConstantValue>(
            KL_FLOAT_CONST, to_string(f1 - f2), v1->line, v1->col);
      case KL_TT_Operator_Mul:
        return make_unique<ASTExprConstantValue>(
            KL_FLOAT_CONST, to_string(f1 * f2), v1->line, v1->col);
      case KL_TT_Operator_Div:
        return make_unique<ASTExprConstantValue>(
            KL_FLOAT_CONST, to_string(f1 / f2), v1->line, v1->col);
      case KL_TT_Operator_Mod:
        return make_unique<ASTExprConstantValue>(
            KL_FLOAT_CONST, to_string(fmod(f1, f2)), v1->line, v1->col);
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, f1 == f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, f1 != f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Less:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, f1 < f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_LessEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, f1 <= f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Greater:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, f1 > f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_GreaterEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, f1 >= f2 ? "true" : "false", v1->line, v1->col);
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid operator in float expression after semantic "
            "check");
    }
  // If either is an int, then the other must be an int or a char (float has been handled above)
  } else if (lhs->type.primitive == KL_INT_PRIMITIVE || rhs->type.primitive == KL_INT_PRIMITIVE) {
    // v1 is definitely an int
    auto v1 = (lhs->type.primitive == KL_INT_PRIMITIVE) ? lhs : rhs;

    auto v2 = (lhs->type.primitive == KL_INT_PRIMITIVE) ? rhs : lhs;

    int i1 = stoi(v1->value);

    int i2;

    switch (v2->type.primitive) {
      case KL_INT_PRIMITIVE:
        i2 = stoi(v2->value);
        break;
      case KL_CHAR_PRIMITIVE:
        i2 = (unsigned char)v2->value[0];
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid type in int expression after semantic "
            "check");
    }

    switch (binary->op) {
      case KL_TT_Operator_Add:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 + i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_Sub:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 - i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_Mul:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 * i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_Div:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 / i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_Mod:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 % i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, i1 == i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, i1 != i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Less:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, i1 < i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_LessEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, i1 <= i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Greater:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, i1 > i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_GreaterEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, i1 >= i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Shl:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 << i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_Shr:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 >> i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_BitwiseAnd:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 & i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_BitwiseOr:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 | i2),
                                                 v1->line, v1->col);
      case KL_TT_Operator_BitwiseXor:
        return make_unique<ASTExprConstantValue>(KL_INT_CONST, to_string(i1 ^ i2),
                                                 v1->line, v1->col);
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid operator in int expression after semantic "
            "check");
    }
  // If either is a bool, then the other must be a bool
  } else if (lhs->type.primitive == KL_BOOL_PRIMITIVE && rhs->type.primitive == KL_BOOL_PRIMITIVE) {
    bool b1 = lhs->value == "true";
    bool b2 = rhs->value == "true";

    switch (binary->op) {
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, b1 == b2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, b1 != b2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_LogicalAnd:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, b1 && b2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_LogicalOr:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, b1 || b2 ? "true" : "false", lhs->line, lhs->col);
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid operator in bool expression after semantic "
            "check");
    }
  // If either is a char, then the other must be a char (string and int are covered)
  } else if (lhs->type.primitive == KL_CHAR_PRIMITIVE && rhs->type.primitive == KL_CHAR_PRIMITIVE) {
    char c1 = lhs->value[0];
    char c2 = rhs->value[0];

    switch (binary->op) {
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, c1 == c2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, c1 != c2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_Less:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, c1 < c2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_LessEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, c1 <= c2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_Greater:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, c1 > c2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_GreaterEqual:
        return make_unique<ASTExprConstantValue>(
            KL_BOOL_CONST, c1 >= c2 ? "true" : "false", lhs->line, lhs->col);
      case KL_TT_Operator_BitwiseAnd:
        return make_unique<ASTExprConstantValue>(KL_CHAR_CONST, string(1, c1 & c2),
                                                 lhs->line, lhs->col);
      case KL_TT_Operator_BitwiseOr:
        return make_unique<ASTExprConstantValue>(KL_CHAR_CONST, string(1, c1 | c2),
                                                 lhs->line, lhs->col);
      case KL_TT_Operator_BitwiseXor:
        return make_unique<ASTExprConstantValue>(KL_CHAR_CONST, string(1, c1 ^ c2),
                                                 lhs->line, lhs->col);
      default:
        throw std::runtime_error(
            "UNREACHABLE : Invalid operator in char expression after semantic "
            "check");
    }
  }

  throw std::runtime_error(
      "UNREACHABLE : Invalid type combination of binary operator after "
      "semantic check");

  return nullptr;
}