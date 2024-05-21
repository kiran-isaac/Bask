//
// Created by kiran on 5/21/24.
//

#include <tokens.h>

int get_operator_precedence(KL_TokenType type) {
  switch (type) {
    case KL_TT_Operator_Assign:
      return 1;
    case KL_TT_Operator_LogicalOr:
      return 2;
    case KL_TT_Operator_LogicalAnd:
      return 3;
    case KL_TT_Operator_BitwiseOr:
      return 4;
    case KL_TT_Operator_BitwiseXor:
      return 5;
    case KL_TT_Operator_BitwiseAnd:
      return 6;
    case KL_TT_Operator_Equal:
    case KL_TT_Operator_NotEqual:
      return 7;
    case KL_TT_Operator_Less:
    case KL_TT_Operator_LessEqual:
    case KL_TT_Operator_Greater:
    case KL_TT_Operator_GreaterEqual:
      return 8;
    case KL_TT_Operator_Shl:
    case KL_TT_Operator_Shr:
      return 9;
    case KL_TT_Operator_Add:
    case KL_TT_Operator_Sub:
      return 10;
    case KL_TT_Operator_Mul:
    case KL_TT_Operator_Div:
    case KL_TT_Operator_Mod:
      return 11;
    case KL_TT_Operator_BitwiseNot:
    case KL_TT_Operator_LogicalNot:
      return 12;
    default:
      return -1;
  }
}

bool operator_is_commutative(KL_TokenType op) {
  return op == KL_TT_Operator_Add || op == KL_TT_Operator_Mul || op == KL_TT_Operator_Equal || op == KL_TT_Operator_NotEqual;
}