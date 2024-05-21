//
// Created by kiran on 5/20/24.
//

#ifndef KL_TOKENS_H
#define KL_TOKENS_H

#include <memory>
#include <string>

using namespace std;

enum KLTokenType {
  KLTT_Identifier,
  KLTT_EndOfFile,
  
  KLTT_Literal_Int,
  KLTT_Literal_Float,
  KLTT_Literal_String,
  KLTT_Literal_Char,
  KLTT_Literal_Bool,
  
  KLTT_Punctuation_Comma,
  KLTT_Punctuation_Semicolon,
  KLTT_Punctuation_Colon,
  KLTT_Punctuation_Dot,
  KLTT_Punctuation_LParen,
  KLTT_Punctuation_RParen,
  KLTT_Punctuation_LBrace,
  KLTT_Punctuation_RBrace,
  KLTT_Punctuation_LBracket,
  KLTT_Punctuation_RBracket,
  
  KLTT_Operator_Add,
  KLTT_Operator_Sub,
  KLTT_Operator_Mul,
  KLTT_Operator_Div,
  KLTT_Operator_Mod,
  KLTT_Operator_BitwiseAnd,
  KLTT_Operator_BitwiseOr,
  KLTT_Operator_BitwiseXor,
  KLTT_Operator_BitwiseNot,
  KLTT_Operator_LogicalAnd,
  KLTT_Operator_LogicalOr,
  KLTT_Operator_LogicalNot,
  KLTT_Operator_Assign,
  KLTT_Operator_Equal,
  KLTT_Operator_NotEqual,
  KLTT_Operator_Less,
  KLTT_Operator_LessEqual,
  KLTT_Operator_Greater,
  KLTT_Operator_GreaterEqual,
  KLTT_Operator_Shl,
  KLTT_Operator_Shr,
  
  KLTT_KW_Int,
  KLTT_KW_Float,
  KLTT_KW_Char,
  KLTT_KW_String,
  KLTT_KW_Bool,
  KLTT_KW_Const,
  KLTT_KW_Void,
  
  KLTT_KW_If,
  KLTT_KW_Else,
  KLTT_KW_For,
  KLTT_KW_While,
  KLTT_KW_Break,
  KLTT_KW_Return,
};

struct Token {
  KLTokenType type;
  string value;
  unsigned int line;
  unsigned int col;
};

int getOperatorPrecedence(KLTokenType type);
bool isCommutative(KLTokenType op);

const char *tokenTypeToString(KLTokenType type);

string tokenToString(const Token& token);

#endif //KL_TOKENS_H
