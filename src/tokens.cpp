//
// Created by kiran on 5/21/24.
//

#include <tokens.h>

int getOperatorPrecedence(KLTokenType type) {
  switch (type) {
    case KLTT_Operator_Assign:
      return 1;
    case KLTT_Operator_LogicalOr:
      return 2;
    case KLTT_Operator_LogicalAnd:
      return 3;
    case KLTT_Operator_BitwiseOr:
      return 4;
    case KLTT_Operator_BitwiseXor:
      return 5;
    case KLTT_Operator_BitwiseAnd:
      return 6;
    case KLTT_Operator_Equal:
    case KLTT_Operator_NotEqual:
      return 7;
    case KLTT_Operator_Less:
    case KLTT_Operator_LessEqual:
    case KLTT_Operator_Greater:
    case KLTT_Operator_GreaterEqual:
      return 8;
    case KLTT_Operator_Shl:
    case KLTT_Operator_Shr:
      return 9;
    case KLTT_Operator_Add:
    case KLTT_Operator_Sub:
      return 10;
    case KLTT_Operator_Mul:
    case KLTT_Operator_Div:
    case KLTT_Operator_Mod:
      return 11;
    case KLTT_Operator_BitwiseNot:
    case KLTT_Operator_LogicalNot:
      return 12;
    default:
      return -1;
  }
}

bool isCommutative(KLTokenType op) {
  return op == KLTT_Operator_Add || op == KLTT_Operator_Mul || op == KLTT_Operator_Equal || op == KLTT_Operator_NotEqual;
}