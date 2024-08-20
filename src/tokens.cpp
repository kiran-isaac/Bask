//
// Created by kiran on 5/21/24.
//

#include <tokens.h>

int get_operator_precedence(BASK_TokenType type) {
  switch (type) {
    case BASK_TT_Operator_Assign:
      return 1;
    case BASK_TT_Operator_LogicalOr:
      return 2;
    case BASK_TT_Operator_LogicalAnd:
      return 3;
    case BASK_TT_Operator_BitwiseOr:
      return 4;
    case BASK_TT_Operator_BitwiseXor:
      return 5;
    case BASK_TT_Operator_BitwiseAnd:
      return 6;
    case BASK_TT_Operator_Equal:
    case BASK_TT_Operator_NotEqual:
      return 7;
    case BASK_TT_Operator_Less:
    case BASK_TT_Operator_LessEqual:
    case BASK_TT_Operator_Greater:
    case BASK_TT_Operator_GreaterEqual:
      return 8;
    case BASK_TT_Operator_Shl:
    case BASK_TT_Operator_Shr:
      return 9;
    case BASK_TT_Operator_Add:
    case BASK_TT_Operator_Sub:
      return 10;
    case BASK_TT_Operator_Mul:
    case BASK_TT_Operator_Div:
    case BASK_TT_Operator_Mod:
      return 11;
    case BASK_TT_Operator_BitwiseNot:
    case BASK_TT_Operator_LogicalNot:
      return 12;
    default:
      return -1;
  }
}

bool operator_is_commutative(BASK_TokenType op) {
  return op == BASK_TT_Operator_Add || op == BASK_TT_Operator_Mul || op == BASK_TT_Operator_Equal || op == BASK_TT_Operator_NotEqual;
}