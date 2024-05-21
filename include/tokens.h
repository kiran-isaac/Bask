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
  KL_TT_Operator_Assign,
  KL_TT_Operator_Equal,
  KL_TT_Operator_NotEqual,
  KL_TT_Operator_Less,
  KL_TT_Operator_LessEqual,
  KL_TT_Operator_Greater,
  KL_TT_Operator_GreaterEqual,
  KL_TT_Operator_Shl,
  KL_TT_Operator_Shr,
  
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

struct KL_Token {
  KL_TokenType type;
  string value;
  unsigned int line;
  unsigned int col;
};

int getOperatorPrecedence(KL_TokenType type);
bool isCommutative(KL_TokenType op);

const char *tokenTypeToString(KL_TokenType type);

string tokenToString(const KL_Token& token);

#endif //KL_TOKENS_H
