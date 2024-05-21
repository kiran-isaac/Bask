//
// Created by kiran on 5/20/24.
//

#include <tokens.h>

string token_to_string(const KL_Token& token) {
  return "TK:" + string(token_type_to_string(token.type)) + " " + token.value + " [" + to_string(token.line) + ":" + to_string(token.col) + "]";
}

const char* token_type_to_string(KL_TokenType type)  {
  switch (type) {
    case KL_TT_Identifier:
      return "Identifier";
    case KL_TT_EndOfFile:
      return "EndOfFile";
      
    case KL_TT_Literal_Int:
      return "Literal_Integer";
    case KL_TT_Literal_Float:
      return "Literal_Float";
    case KL_TT_Literal_String:
      return "Literal_String";
    case KL_TT_Literal_Char:
      return "Literal_Char";
    case KL_TT_Literal_Bool:
      return "Literal_Bool";
      
    case KL_TT_Punctuation_Comma:
      return "Punctuation_Comma";
    case KL_TT_Punctuation_Semicolon:
      return "Punctuation_Semicolon";
    case KL_TT_Punctuation_Colon:
      return "Punctuation_Colon";
    case KL_TT_Punctuation_Dot:
      return "Punctuation_Dot";
    case KL_TT_Punctuation_LParen:
      return "Punctuation_LParen";
    case KL_TT_Punctuation_RParen:
      return "Punctuation_RParen";
    case KL_TT_Punctuation_LBrace:
      return "Punctuation_LBrace";
    case KL_TT_Punctuation_RBrace:
      return "Punctuation_RBrace";
    case KL_TT_Punctuation_LBracket:
      return "Punctuation_LBracket";
    case KL_TT_Punctuation_RBracket:
      return "Punctuation_RBracket";
      
    case KL_TT_Operator_Add:
      return "Operator_Add";
    case KL_TT_Operator_Sub:
      return "Operator_Sub";
    case KL_TT_Operator_Mul:
      return "Operator_Mul";
    case KL_TT_Operator_Div:
      return "Operator_Div";
    case KL_TT_Operator_Mod:
      return "Operator_Mod";
    case KL_TT_Operator_BitwiseAnd:
      return "Operator_BitwiseAnd";
    case KL_TT_Operator_BitwiseOr:
      return "Operator_BitwiseOr";
    case KL_TT_Operator_BitwiseXor:
      return "Operator_BitwiseXor";
    case KL_TT_Operator_BitwiseNot:
      return "Operator_BitwiseNot";
    case KL_TT_Operator_LogicalAnd:
      return "Operator_LogicalAnd";
    case KL_TT_Operator_LogicalOr:
      return "Operator_LogicalOr";
    case KL_TT_Operator_LogicalNot:
      return "Operator_LogicalNot";
    case KL_TT_Operator_Assign:
      return "Operator_Assign";
    case KL_TT_Operator_Equal:
      return "Operator_Equal";
    case KL_TT_Operator_NotEqual:
      return "Operator_NotEqual";
    case KL_TT_Operator_Less:
      return "Operator_Less";
    case KL_TT_Operator_LessEqual:
      return "Operator_LessEqual";
    case KL_TT_Operator_Greater:
      return "Operator_Greater";
    case KL_TT_Operator_GreaterEqual:
      return "Operator_GreaterEqual";
    case KL_TT_Operator_Shl:
      return "Operator_Shl";
    case KL_TT_Operator_Shr:
      return "Operator_Shr";
      
    case KL_TT_KW_Int:
      return "KW_Int";
    case KL_TT_KW_Float:
      return "KW_Float";
    case KL_TT_KW_Char:
      return "KW_Char";
    case KL_TT_KW_String:
      return "KW_String";
    case KL_TT_KW_Bool:
      return "KW_Bool";
    case KL_TT_KW_Const:
      return "KW_Const";
    case KL_TT_KW_Void:
      return "KW_Void";
      
    case KL_TT_KW_If:
      return "KW_If";
    case KL_TT_KW_Else:
      return "KW_Else";
    case KL_TT_KW_For:
      return "KW_For";
    case KL_TT_KW_While:
      return "KW_While";
    case KL_TT_KW_Break:
      return "KW_Break";
    case KL_TT_KW_Return:
      return "KW_Return";
  }
}