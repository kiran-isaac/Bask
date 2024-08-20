//
// Created by kiran on 5/20/24.
//

#ifndef BASK_TOKENS_H
#define BASK_TOKENS_H

#include <memory>
#include <string>

using namespace std;

enum BASK_TokenType {
  BASK_TT_Identifier = 0,
  BASK_TT_EndOfFile = 1,

  BASK_TT_Literal_Int = 2,
  BASK_TT_Literal_Float = 3,
  BASK_TT_Literal_String = 4,
  BASK_TT_Literal_Char = 5,
  BASK_TT_Literal_Bool = 6,

  BASK_TT_Punctuation_Comma = 7,
  BASK_TT_Punctuation_Semicolon = 8,
  BASK_TT_Punctuation_Colon = 9,
  BASK_TT_Punctuation_Dot = 10,
  BASK_TT_Punctuation_LParen = 11,
  BASK_TT_Punctuation_RParen = 12,
  BASK_TT_Punctuation_LBrace = 13,
  BASK_TT_Punctuation_RBrace = 14,
  BASK_TT_Punctuation_LBracket = 15,
  BASK_TT_Punctuation_RBracket = 16,

  BASK_TT_Operator_Add = 17,
  BASK_TT_Operator_Sub = 18,
  BASK_TT_Operator_Mul = 19,
  BASK_TT_Operator_Div = 20,
  BASK_TT_Operator_Mod = 21,
  BASK_TT_Operator_BitwiseAnd = 22,
  BASK_TT_Operator_BitwiseOr = 23,
  BASK_TT_Operator_BitwiseXor = 24,
  BASK_TT_Operator_BitwiseNot = 25,
  BASK_TT_Operator_LogicalAnd = 26,
  BASK_TT_Operator_LogicalOr = 27,
  BASK_TT_Operator_LogicalNot = 28,
  BASK_TT_Operator_Equal = 29,
  BASK_TT_Operator_NotEqual = 30,
  BASK_TT_Operator_Less = 31,
  BASK_TT_Operator_LessEqual = 32,
  BASK_TT_Operator_Greater = 33,
  BASK_TT_Operator_GreaterEqual = 34,
  BASK_TT_Operator_Shl = 35,
  BASK_TT_Operator_Shr = 36,

  BASK_TT_Operator_Assign = 37,

  BASK_TT_KW_Int = 38,
  BASK_TT_KW_Float = 39,
  BASK_TT_KW_Char = 40,
  BASK_TT_KW_String = 41,
  BASK_TT_KW_Bool = 42,
  BASK_TT_KW_Const = 43,
  BASK_TT_KW_Void = 44,

  BASK_TT_KW_If = 45,
  BASK_TT_KW_Else = 46,
  BASK_TT_KW_For = 47,
  BASK_TT_KW_While = 48,
  BASK_TT_KW_Break = 49,
  BASK_TT_KW_Return = 50,
  BASK_TT_KW_Continue = 51,
  BASK_TT_KW_Use = 52,
};


// The unary operators are:
// - (negation)
// ! (logical not)
// ~ (bitwise not)
// The binary operators are:
// + (addition)
// - (subtraction)
// * (multiplication)
// / (division)
// % (modulus)
// & (bitwise and)
// | (bitwise or)
// ^ (bitwise xor)
// && (logical and)
// || (logical or)
// == (equal)
// != (not equal)
// < (less than)
// <= (less than or equal)
// > (greater than)
// >= (greater than or equal)
// << (shift left)
// >> (shift right)

#define BIN_OP_CASES \
  BASK_TT_Operator_Add: \
  case BASK_TT_Operator_Sub: \
  case BASK_TT_Operator_Mul: \
  case BASK_TT_Operator_Div: \
  case BASK_TT_Operator_Mod: \
  case BASK_TT_Operator_BitwiseAnd: \
  case BASK_TT_Operator_BitwiseOr: \
  case BASK_TT_Operator_BitwiseXor: \
  case BASK_TT_Operator_LogicalAnd: \
  case BASK_TT_Operator_LogicalOr: \
  case BASK_TT_Operator_Equal: \
  case BASK_TT_Operator_NotEqual: \
  case BASK_TT_Operator_Less: \
  case BASK_TT_Operator_LessEqual: \
  case BASK_TT_Operator_Greater: \
  case BASK_TT_Operator_GreaterEqual: \
  case BASK_TT_Operator_Shl: \
  case BASK_TT_Operator_Shr

#define UN_OP_CASES \
  BASK_TT_Operator_Sub: \
  case BASK_TT_Operator_LogicalNot: \
  case BASK_TT_Operator_BitwiseNot

#define LITERAL_CASES \
  BASK_TT_Literal_Int: \
  case BASK_TT_Literal_Float: \
  case BASK_TT_Literal_String: \
  case BASK_TT_Literal_Char: \
  case BASK_TT_Literal_Bool

#define ARITHMETIC_BINARY_CASES \
  BASK_TT_Operator_Add: \
  case BASK_TT_Operator_Sub: \
  case BASK_TT_Operator_Mul: \
  case BASK_TT_Operator_Div: \
  case BASK_TT_Operator_Mod

#define COMPARISON_BINARY_CASES \
  BASK_TT_Operator_Equal: \
  case BASK_TT_Operator_NotEqual: \
  case BASK_TT_Operator_Less: \
  case BASK_TT_Operator_LessEqual: \
  case BASK_TT_Operator_Greater: \
  case BASK_TT_Operator_GreaterEqual 

#define LOGICAL_BINARY_CASES \
  BASK_TT_Operator_LogicalAnd: \
  case BASK_TT_Operator_LogicalOr \


#define BITWISE_BINARY_CASES \
  BASK_TT_Operator_BitwiseAnd: \
  case BASK_TT_Operator_BitwiseOr: \
  case BASK_TT_Operator_BitwiseXor: \

#define SHIFT_BINARY_CASES \
  BASK_TT_Operator_Shl: \
  case BASK_TT_Operator_Shr

struct BASK_Token {
  BASK_TokenType type;
  string value;
  unsigned int line;
  unsigned int col;
};

int get_operator_precedence(BASK_TokenType type);
bool operator_is_commutative(BASK_TokenType op);

const char *token_type_to_string(BASK_TokenType type);
string token_to_string(const BASK_Token& token);

#endif //BASK_TOKENS_H
