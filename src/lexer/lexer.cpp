//
// Created by kiran on 5/20/24.
//

#include "lexer.h"

#define UNKNOWN_TOKEN_ERROR_LOCATION_SIZE 10

void Lexer::refresh_buffer_1() {
  memset(buf1, EOF, 4096);
  fread(buf1, 1, 4096, file);
}

void Lexer::refresh_buffer_2() {
  memset(buf2, EOF, 4096);
  fread(buf2, 1, 4096, file);
}

void Lexer::advance() {
  if (c == buf1 + 4095) {
    refresh_buffer_2();
    c = buf2;
    isBuffer1 = false;
  } else if (c == buf2 + 4095) {
    refresh_buffer_1();
    c = buf1;
    isBuffer1 = true;
  } else {
    c++;
  }
  col++;
}

Lexer::Lexer(const Options& options) {
  file = fopen(options.file.c_str(), "r");
  if (file == nullptr) {
    lexerError = "Error: could not open file " + options.file;
    return;
  }
  
  refresh_buffer_1();
  
  c = buf1;
  line = 1;
  col = 1;
  isBuffer1 = true;
  
  preprocessor_condition_depth = 0;
  pathway_found = false;
}

optional<KL_Token> Lexer::lex_word() {
  string word;
  word += *c;
  advance();
  
  while (isalnum(*c) || *c == '_') {
    word += *c;
    advance();
  }
  
  switch (word[0]) {
    case 'b':
      if (word == "break") {
        return KL_Token{KL_TokenType::KL_TT_KW_Break, word, tokenStartLine, tokenStartCol};
      } else if (word == "bool") {
        return KL_Token{KL_TokenType::KL_TT_KW_Bool, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'c':
      if (word == "char") {
        return KL_Token{KL_TokenType::KL_TT_KW_Char, word, tokenStartLine, tokenStartCol};
      } else if (word == "const") {
        return KL_Token{KL_TokenType::KL_TT_KW_Const, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'e':
      if (word == "else") {
        return KL_Token{KL_TokenType::KL_TT_KW_Else, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'f':
      if (word == "for") {
        return KL_Token{KL_TokenType::KL_TT_KW_For, word, tokenStartLine, tokenStartCol};
      } else if (word == "float") {
        return KL_Token{KL_TokenType::KL_TT_KW_Float, word, tokenStartLine, tokenStartCol};
      } else if (word == "false") {
        return KL_Token{KL_TokenType::KL_TT_Literal_Bool, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'i':
      if (word == "if") {
        return KL_Token{KL_TokenType::KL_TT_KW_If, word, tokenStartLine, tokenStartCol};
      } else if (word == "int") {
        return KL_Token{KL_TokenType::KL_TT_KW_Int, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'r':
      if (word == "return") {
        return KL_Token{KL_TokenType::KL_TT_KW_Return, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 's':
      if (word == "string") {
        return KL_Token{KL_TokenType::KL_TT_KW_String, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 't':
      if (word == "true") {
        return KL_Token{KL_TokenType::KL_TT_Literal_Bool, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'v':
      if (word == "void") {
        return KL_Token{KL_TokenType::KL_TT_KW_Void, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    case 'w':
      if (word == "while") {
        return KL_Token{KL_TokenType::KL_TT_KW_While, word, tokenStartLine, tokenStartCol};
      }
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
    default:
      return KL_Token{KL_TokenType::KL_TT_Identifier, word, tokenStartLine, tokenStartCol};
  }
  
  return nullopt;
}

optional<KL_Token> Lexer::next() {
  while (true) {
    tokenStartCol = col;
    tokenStartLine = line;
    switch (*c) {
      case '\n':
        line++;
        col = 0; // col will be incremented by advance()
      case ' ':
      case '\t':
        advance();
        continue;
        
      case EOF:
        return KL_Token{KL_TokenType::KL_TT_EndOfFile, "", tokenStartLine, tokenStartCol};
      
      case ',':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_Comma, ",", tokenStartLine, tokenStartCol};
      case ';':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_Semicolon, ";", tokenStartLine, tokenStartCol};
      case ':':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_Colon, ":", tokenStartLine, tokenStartCol};
      case '.':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_Dot, ".", tokenStartLine, tokenStartCol};
      case '(':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_LParen, "(", tokenStartLine, tokenStartCol};
      case ')':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_RParen, ")", tokenStartLine, tokenStartCol};
      case '{':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_LBrace, "{", tokenStartLine, tokenStartCol};
      case '}':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_RBrace, "}", tokenStartLine, tokenStartCol};
      case '[':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_LBracket, "[", tokenStartLine, tokenStartCol};
      case ']':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Punctuation_RBracket, "]", tokenStartLine, tokenStartCol};
      
      case '+':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_Add, "+", tokenStartLine, tokenStartCol};
      case '-':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_Sub, "-", tokenStartLine, tokenStartCol};
      case '*':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_Mul, "*", tokenStartLine, tokenStartCol};
      case '/':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_Div, "/", tokenStartLine, tokenStartCol};
      case '%':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_Mod, "%", tokenStartLine, tokenStartCol};
      case '&':
        advance();
        if (*c == '&') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_LogicalAnd, "&&", tokenStartLine, tokenStartCol};
        }
        return KL_Token{KL_TokenType::KL_TT_Operator_BitwiseAnd, "&", tokenStartLine, tokenStartCol};
      case '|':
        advance();
        if (*c == '|') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_LogicalOr, "||", tokenStartLine, tokenStartCol};
        }
        return KL_Token{KL_TokenType::KL_TT_Operator_BitwiseOr, "|", tokenStartLine, tokenStartCol};
      case '^':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_BitwiseXor, "^", tokenStartLine, tokenStartCol};
      case '~':
        advance();
        return KL_Token{KL_TokenType::KL_TT_Operator_BitwiseNot, "~", tokenStartLine, tokenStartCol};
      case '!':
        advance();
        if (*c == '=') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_NotEqual, "!=", tokenStartLine, tokenStartCol};
        }
        return KL_Token{KL_TokenType::KL_TT_Operator_LogicalNot, "!", tokenStartLine, tokenStartCol};
      case '=':
        advance();
        if (*c == '=') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_Equal, "==", tokenStartLine, tokenStartCol};
        }
        return KL_Token{KL_TokenType::KL_TT_Operator_Assign, "=", tokenStartLine, tokenStartCol};
      case '<':
        advance();
        if (*c == '=') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_LessEqual, "<=", tokenStartLine, tokenStartCol};
        } else if (*c == '<') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_Shl, "<<", tokenStartLine, tokenStartCol};
        }
        return KL_Token{KL_TokenType::KL_TT_Operator_Less, "<", tokenStartLine, tokenStartCol};
        
      case '>':
        advance();
        if (*c == '=') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_GreaterEqual, ">=", tokenStartLine, tokenStartCol};
        } else if (*c == '>') {
          advance();
          return KL_Token{KL_TokenType::KL_TT_Operator_Shr, ">>", tokenStartLine, tokenStartCol};
        }
        return KL_Token{KL_TokenType::KL_TT_Operator_Greater, ">", tokenStartLine, tokenStartCol};
      
      case '\'':
        return lex_char_literal();
      case '"':
        return lex_string_literal();
      
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
        return lex_word();
        
      case '0'...'9':
        return lex_number();
      
      default:
        return nullopt;
    }
    
    return std::nullopt;
  }
}