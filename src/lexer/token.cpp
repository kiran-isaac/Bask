//
// Created by kiran on 5/20/24.
//

#include <tokens.h>

string token_to_string(const BASK_Token& token) {
  return "TK:" + string(token_type_to_string(token.type)) + " " + token.value + " [" + to_string(token.line) + ":" + to_string(token.col) + "]";
}

const char* token_type_to_string(BASK_TokenType type)  {
  switch (type) {
    case BASK_TT_Identifier:
      return "Identifier";
    case BASK_TT_EndOfFile:
      return "EndOfFile";
      
    case BASK_TT_Literal_Int:
      return "Literal_Integer";
    case BASK_TT_Literal_Float:
      return "Literal_Float";
    case BASK_TT_Literal_String:
      return "Literal_String";
    case BASK_TT_Literal_Char:
      return "Literal_Char";
    case BASK_TT_Literal_Bool:
      return "Literal_Bool";
      
    case BASK_TT_Punctuation_Comma:
      return "Punctuation_Comma";
    case BASK_TT_Punctuation_Semicolon:
      return "Punctuation_Semicolon";
    case BASK_TT_Punctuation_Colon:
      return "Punctuation_Colon";
    case BASK_TT_Punctuation_Dot:
      return "Punctuation_Dot";
    case BASK_TT_Punctuation_LParen:
      return "Punctuation_LParen";
    case BASK_TT_Punctuation_RParen:
      return "Punctuation_RParen";
    case BASK_TT_Punctuation_LBrace:
      return "Punctuation_LBrace";
    case BASK_TT_Punctuation_RBrace:
      return "Punctuation_RBrace";
    case BASK_TT_Punctuation_LBracket:
      return "Punctuation_LBracket";
    case BASK_TT_Punctuation_RBracket:
      return "Punctuation_RBracket";
      
    case BASK_TT_Operator_Add:
      return "Operator_Add";
    case BASK_TT_Operator_Sub:
      return "Operator_Sub";
    case BASK_TT_Operator_Mul:
      return "Operator_Mul";
    case BASK_TT_Operator_Div:
      return "Operator_Div";
    case BASK_TT_Operator_Mod:
      return "Operator_Mod";
    case BASK_TT_Operator_BitwiseAnd:
      return "Operator_BitwiseAnd";
    case BASK_TT_Operator_BitwiseOr:
      return "Operator_BitwiseOr";
    case BASK_TT_Operator_BitwiseXor:
      return "Operator_BitwiseXor";
    case BASK_TT_Operator_BitwiseNot:
      return "Operator_BitwiseNot";
    case BASK_TT_Operator_LogicalAnd:
      return "Operator_LogicalAnd";
    case BASK_TT_Operator_LogicalOr:
      return "Operator_LogicalOr";
    case BASK_TT_Operator_LogicalNot:
      return "Operator_LogicalNot";
    case BASK_TT_Operator_Assign:
      return "Operator_Assign";
    case BASK_TT_Operator_Equal:
      return "Operator_Equal";
    case BASK_TT_Operator_NotEqual:
      return "Operator_NotEqual";
    case BASK_TT_Operator_Less:
      return "Operator_Less";
    case BASK_TT_Operator_LessEqual:
      return "Operator_LessEqual";
    case BASK_TT_Operator_Greater:
      return "Operator_Greater";
    case BASK_TT_Operator_GreaterEqual:
      return "Operator_GreaterEqual";
    case BASK_TT_Operator_Shl:
      return "Operator_Shl";
    case BASK_TT_Operator_Shr:
      return "Operator_Shr";
      
    case BASK_TT_KW_Int:
      return "KW_Int";
    case BASK_TT_KW_Float:
      return "KW_Float";
    case BASK_TT_KW_Char:
      return "KW_Char";
    case BASK_TT_KW_String:
      return "KW_String";
    case BASK_TT_KW_Bool:
      return "KW_Bool";
    case BASK_TT_KW_Const:
      return "KW_Const";
    case BASK_TT_KW_Void:
      return "KW_Void";
      
    case BASK_TT_KW_If:
      return "KW_If";
    case BASK_TT_KW_Else:
      return "KW_Else";
    case BASK_TT_KW_For:
      return "KW_For";
    case BASK_TT_KW_While:
      return "KW_While";
    case BASK_TT_KW_Break:
      return "KW_Break";
    case BASK_TT_KW_Return:
      return "KW_Return";
  }
}