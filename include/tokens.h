//
// Created by kiran on 5/20/24.
//

#ifndef KL_TOKENS_H
#define KL_TOKENS_H

#include <memory>
#include <string>

using namespace std;

enum KL_TokenType {
  KL_TT_Identifier,
  KL_TT_EndOfFile,

  KL_TT_Literal_Int,
  KL_TT_Literal_Float,
  KL_TT_Literal_String,
  KL_TT_Literal_Char,
  KL_TT_Literal_Bool,

  KL_TT_Punctuation_Comma,
  KL_TT_Punctuation_Semicolon,
  KL_TT_Punctuation_Colon,
  KL_TT_Punctuation_Dot,
  KL_TT_Punctuation_LParen,
  KL_TT_Punctuation_RParen,
  KL_TT_Punctuation_LBrace,
  KL_TT_Punctuation_RBrace,
  KL_TT_Punctuation_LBracket,
  KL_TT_Punctuation_RBracket,

  KL_TT_Operator_Add,
  KL_TT_Operator_Sub,
  KL_TT_Operator_Mul,
  KL_TT_Operator_Div,
  KL_TT_Operator_Mod,
  KL_TT_Operator_BitwiseAnd,
  KL_TT_Operator_BitwiseOr,
  KL_TT_Operator_BitwiseXor,
  KL_TT_Operator_BitwiseNot,
  KL_TT_Operator_LogicalAnd,
  KL_TT_Operator_LogicalOr,
  KL_TT_Operator_LogicalNot,
  KL_TT_Operator_Equal,
  KL_TT_Operator_NotEqual,
  KL_TT_Operator_Less,
  KL_TT_Operator_LessEqual,
  KL_TT_Operator_Greater,
  KL_TT_Operator_GreaterEqual,
  KL_TT_Operator_Shl,
  KL_TT_Operator_Shr,

  KL_TT_Operator_Assign,

  KL_TT_KW_Int,
  KL_TT_KW_Float,
  KL_TT_KW_Char,
  KL_TT_KW_String,
  KL_TT_KW_Bool,
  KL_TT_KW_Const,
  KL_TT_KW_Void,

  KL_TT_KW_If,
  KL_TT_KW_Else,
  KL_TT_KW_For,
  KL_TT_KW_While,
  KL_TT_KW_Break,
  KL_TT_KW_Return,
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
