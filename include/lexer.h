//
// Created by kiran on 5/20/24.
//

#ifndef KL_LEXER_H
#define KL_LEXER_H

#include <memory>
#include <string>
#include <optional>
#include <cstdio>
#include <cstring>
#include <options.h>
#include <tokens.h>

using namespace std;

class Lexer {
public:
  explicit Lexer(const Options& options);
  
  string lexerError;
  
  optional<KL_Token> next();
private:
  FILE *file{};
  
  char buf1[4096]{};
  char buf2[4096]{};
  
  char *c;
  
  unsigned int line;
  unsigned int col;
  unsigned int tokenStartLine;
  unsigned int tokenStartCol;
  
  bool isBuffer1;
  
  void refresh_buffer_1();
  void refresh_buffer_2();
  
  unsigned int preprocessor_condition_depth;
  bool pathway_found; // used
  
  void advance();
  
  optional<KL_Token> lex_word();
  optional<KL_Token> lex_number();
  optional<KL_Token> lex_string_literal();
  optional<KL_Token> lex_char_literal();
  optional<char> let_escape_char();
};

#endif //KL_LEXER_H
