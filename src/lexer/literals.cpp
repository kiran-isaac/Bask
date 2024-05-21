//
// Created by kiran on 5/20/24.
//
#include "lexer.h"

std::optional<char> Lexer::let_escape_char() {
  int val = 0;
  int i = 0; // counter for octal escape sequences
  switch (*c) {
    case 'a':
      advance();
      return '\a';
    case 'b':
      advance();
      return '\b';
    case 'f':
      advance();
      return '\f';
    case 'n':
      advance();
      return '\n';
    case 'r':
      advance();
      return '\r';
    case 't':
      advance();
      return '\t';
    case 'v':
      advance();
      return '\v';
    case '\\':
      advance();
      return '\\';
    case '\'':
      advance();
      return '\'';
    case '"':
      advance();
      return '"';
    case '?':
      advance();
      return '?';
    case '0':
      advance();
      if (!isdigit(*c)) {
        return '\0';
      } else {
        val = 0;
        i = 0;
        while (isdigit(*c) && *c < '8') {
          val *= 8;
          val += *c - '0';
          advance();
          i++;
          
          if (i > 2) {
            lexerError = "Invalid octal escape sequence";
            return std::nullopt;
          }
        }
        
        if (val > 255) {
          lexerError = "Invalid octal escape sequence";
          return std::nullopt;
        }
        
        return static_cast<char>(val);
      }
    
    case 'x':
      advance();
      if (!isxdigit(*c)) {
        lexerError = "Invalid hexadecimal escape sequence";
        return -1;
      }
      while (isxdigit(*c)) {
        val *= 16;
        if (isdigit(*c)) {
          val += *c - '0';
        } else {
          val += tolower(*c) - 'a' + 10;
        }
        advance();
        
        if (val > 255) {
          lexerError = "Invalid hexadecimal escape sequence";
          return std::nullopt;
        }
      }
      return static_cast<char>(val);
    
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      val = 0;
      i = 0;
      while (isdigit(*c) && *c < '8') {
        val *= 8;
        val += *c - '0';
        advance();
        i++;
        
        if (val > 255 || i > 3) {
          lexerError = "Invalid octal escape sequence";
          return std::nullopt;
        }
      }
      return static_cast<char>(val);
    
    default:
      lexerError = "Invalid escape sequence";
      return std::nullopt;
  }
}

std::optional<KL_Token> Lexer::lex_string_literal() {
  advance();
  std::string str;
  while (*c != '"') {
    if (*c == '\\') {
      advance();
      auto escapeChar = let_escape_char();
      if (escapeChar.has_value()) {
        str += escapeChar.value();
      } else {
        return std::nullopt;
      }
    } else {
      str += *c;
      advance();
    }
  }
  advance();
  return KL_Token{KL_TokenType::KL_TT_Literal_String, str, tokenStartLine, tokenStartCol};
}

std::optional<KL_Token> Lexer::lex_char_literal() {
  string charLiteralString = "";
  advance();
  if (*c == '\\') {
    advance();
    auto escapeChar = let_escape_char();
    if (escapeChar.has_value()) {
      if (*c == '\'') {
        advance();
        charLiteralString += escapeChar.value();
        return KL_Token{KL_TokenType::KL_TT_Literal_Char, charLiteralString, tokenStartLine, tokenStartCol};
      }
    }
  } else {
    auto escapeChar = *c;
    advance();
    if (*c == '\'') {
      advance();
      charLiteralString += escapeChar;
      return KL_Token{KL_TokenType::KL_TT_Literal_Char, charLiteralString, tokenStartLine, tokenStartCol};
    }
  }
  
  lexerError = "Invalid character literal";
  return std::nullopt;
}

std::optional<KL_Token> Lexer::lex_number() {
  std::string num;
  while (isdigit(*c)) {
    num += *c;
    advance();
  }
  
  if (*c == '.') {
    num += *c;
    advance();
    while (isdigit(*c)) {
      num += *c;
      advance();
    }
    return KL_Token{KL_TokenType::KL_TT_Literal_Float, num, line, col};
  }
  
  return KL_Token{KL_TokenType::KL_TT_Literal_Int, num, line, col};
}