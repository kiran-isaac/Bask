//
// Created by kiran on 5/20/24.
//

#include <tokens.h>

string tokenToString(const Token& token) {
  return "TK:" + string(tokenTypeToString(token.type)) + " " + token.value + " [" + to_string(token.line) + ":" + to_string(token.col) + "]";
}

const char* tokenTypeToString(KLTokenType type)  {
  switch (type) {
    case KLTT_Identifier:
      return "Identifier";
    case KLTT_EndOfFile:
      return "EndOfFile";
    case KLTT_Literal_Integer:
      return "Literal_Integer";
    case KLTT_Literal_Float:
      return "Literal_Float";
    case KLTT_Literal_String:
      return "Literal_String";
    case KLTT_Literal_Char:
      return "Literal_Char";
    case KLTT_Literal_Bool:
      return "Literal_Bool";
    case KLTT_Punctuation_Comma:
      return "Punctuation_Comma";
    case KLTT_Punctuation_Semicolon:
      return "Punctuation_Semicolon";
    case KLTT_Punctuation_Colon:
      return "Punctuation_Colon";
    case KLTT_Punctuation_Dot:
      return "Punctuation_Dot";
    case KLTT_Punctuation_LParen:
      return "Punctuation_LParen";
    case KLTT_Punctuation_RParen:
      return "Punctuation_RParen";
    case KLTT_Punctuation_LBrace:
      return "Punctuation_LBrace";
    case KLTT_Punctuation_RBrace:
      return "Punctuation_RBrace";
    case KLTT_Punctuation_LBracket:
      return "Punctuation_LBracket";
    case KLTT_Punctuation_RBracket:
      return "Punctuation_RBracket";
    case KLTT_Operator_Add:
      return "Operator_Add";
    case KLTT_Operator_Sub:
      return "Operator_Sub";
    case KLTT_Operator_Mul:
      return "Operator_Mul";
    case KLTT_Operator_Div:
      return "Operator_Div";
    case KLTT_Operator_Mod:
      return "Operator_Mod";
    case KLTT_Operator_BitwiseAnd:
      return "Operator_BitwiseAnd";
    case KLTT_Operator_BitwiseOr:
      return "Operator_BitwiseOr";
    case KLTT_Operator_BitwiseXor:
      return "Operator_BitwiseXor";
    case KLTT_Operator_BitwiseNot:
      return "Operator_BitwiseNot";
    case KLTT_Operator_LogicalAnd:
      return "Operator_LogicalAnd";
    case KLTT_Operator_LogicalOr:
      return "Operator_LogicalOr";
    case KLTT_Operator_LogicalNot:
      return "Operator_LogicalNot";
    case KLTT_Operator_Assign:
      return "Operator_Assign";
    case KLTT_KW_Int:
      return "KW_Int";
    case KLTT_KW_Float:
      return "KW_Float";
    case KLTT_KW_Char:
      return "KW_Char";
    case KLTT_KW_String:
      return "KW_String";
    case KLTT_KW_Bool:
      return "KW_Bool";
    case KLTT_KW_Const:
      return "KW_Const";
    case KLTT_KW_If:
      return "KW_If";
    case KLTT_KW_Else:
      return "KW_Else";
    case KLTT_KW_For:
      return "KW_For";
    case KLTT_KW_While:
      return "KW_While";
    case KLTT_KW_Break:
      return "KW_Break";
    case KLTT_KW_Return:
      return "KW_Return";
  }
}