//
// Created by kiran on 5/20/24.
//

#ifndef KL_TOKENS_H
#define KL_TOKENS_H

#include <memory>
#include <string>

using namespace std;

enum KL_TokenType {
  KL_TT_Identifier = 0,
  KL_TT_EndOfFile = 1,

  KL_TT_Literal_Int = 2,
  KL_TT_Literal_Float = 3,
  KL_TT_Literal_String = 4,
  KL_TT_Literal_Char = 5,
  KL_TT_Literal_Bool = 6,

  KL_TT_Punctuation_Comma = 7,
  KL_TT_Punctuation_Semicolon = 8,
  KL_TT_Punctuation_Colon = 9,
  KL_TT_Punctuation_Dot = 10,
  KL_TT_Punctuation_LParen = 11,
  KL_TT_Punctuation_RParen = 12,
  KL_TT_Punctuation_LBrace = 13,
  KL_TT_Punctuation_RBrace = 14,
  KL_TT_Punctuation_LBracket = 15,
  KL_TT_Punctuation_RBracket = 16,

  KL_TT_Operator_Add = 17,
  KL_TT_Operator_Sub = 18,
  KL_TT_Operator_Mul = 19,
  KL_TT_Operator_Div = 20,
  KL_TT_Operator_Mod = 21,
  KL_TT_Operator_BitwiseAnd = 22,
  KL_TT_Operator_BitwiseOr = 23,
  KL_TT_Operator_BitwiseXor = 24,
  KL_TT_Operator_BitwiseNot = 25,
  KL_TT_Operator_LogicalAnd = 26,
  KL_TT_Operator_LogicalOr = 27,
  KL_TT_Operator_LogicalNot = 28,
  KL_TT_Operator_Equal = 29,
  KL_TT_Operator_NotEqual = 30,
  KL_TT_Operator_Less = 31,
  KL_TT_Operator_LessEqual = 32,
  KL_TT_Operator_Greater = 33,
  KL_TT_Operator_GreaterEqual = 34,
  KL_TT_Operator_Shl = 35,
  KL_TT_Operator_Shr = 36,

  KL_TT_Operator_Assign = 37,

  KL_TT_KW_Int = 38,
  KL_TT_KW_Float = 39,
  KL_TT_KW_Char = 40,
  KL_TT_KW_String = 41,
  KL_TT_KW_Bool = 42,
  KL_TT_KW_Const = 43,
  KL_TT_KW_Void = 44,

  KL_TT_KW_If = 45,
  KL_TT_KW_Else = 46,
  KL_TT_KW_For = 47,
  KL_TT_KW_While = 48,
  KL_TT_KW_Break = 49,
  KL_TT_KW_Return = 50,
  KL_TT_KW_Continue = 51,
  KL_TT_KW_Use = 52,
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
  KL_TT_Operator_Add: \
  case KL_TT_Operator_Sub: \
  case KL_TT_Operator_Mul: \
  case KL_TT_Operator_Div: \
  case KL_TT_Operator_Mod: \
  case KL_TT_Operator_BitwiseAnd: \
  case KL_TT_Operator_BitwiseOr: \
  case KL_TT_Operator_BitwiseXor: \
  case KL_TT_Operator_LogicalAnd: \
  case KL_TT_Operator_LogicalOr: \
  case KL_TT_Operator_Equal: \
  case KL_TT_Operator_NotEqual: \
  case KL_TT_Operator_Less: \
  case KL_TT_Operator_LessEqual: \
  case KL_TT_Operator_Greater: \
  case KL_TT_Operator_GreaterEqual: \
  case KL_TT_Operator_Shl: \
  case KL_TT_Operator_Shr

#define UN_OP_CASES \
  KL_TT_Operator_Sub: \
  case KL_TT_Operator_LogicalNot: \
  case KL_TT_Operator_BitwiseNot

#define LITERAL_CASES \
  KL_TT_Literal_Int: \
  case KL_TT_Literal_Float: \
  case KL_TT_Literal_String: \
  case KL_TT_Literal_Char: \
  case KL_TT_Literal_Bool

#define ARITHMETIC_BINARY_CASES \
  KL_TT_Operator_Add: \
  case KL_TT_Operator_Sub: \
  case KL_TT_Operator_Mul: \
  case KL_TT_Operator_Div: \
  case KL_TT_Operator_Mod

#define COMPARISON_BINARY_CASES \
  KL_TT_Operator_Equal: \
  case KL_TT_Operator_NotEqual: \
  case KL_TT_Operator_Less: \
  case KL_TT_Operator_LessEqual: \
  case KL_TT_Operator_Greater: \
  case KL_TT_Operator_GreaterEqual 

#define LOGICAL_BINARY_CASES \
  KL_TT_Operator_LogicalAnd: \
  case KL_TT_Operator_LogicalOr \


#define BITWISE_BINARY_CASES \
  KL_TT_Operator_BitwiseAnd: \
  case KL_TT_Operator_BitwiseOr: \
  case KL_TT_Operator_BitwiseXor: \

#define SHIFT_BINARY_CASES \
  KL_TT_Operator_Shl: \
  case KL_TT_Operator_Shr

struct KL_Token {
  KL_TokenType type;
  string value;
  unsigned int line;
  unsigned int col;
};

int get_operator_precedence(KL_TokenType type);
bool operator_is_commutative(KL_TokenType op);

const char *token_type_to_string(KL_TokenType type);
string token_to_string(const KL_Token& token);

#endif //KL_TOKENS_H
