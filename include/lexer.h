//
// Created by kiran on 5/20/24.
//

#ifndef BASK_LEXER_H
#define BASK_LEXER_H

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
  explicit Lexer(const CommandLineArguments& options);

  explicit Lexer(string filename, const CommandLineArguments& options);
  
  string lexerError;
  
	optional<BASK_Token> next();

  CommandLineArguments options;
private:
  FILE *file{};
  bool isStdIn;
  
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
  
  optional<BASK_Token> lex_word();
  optional<BASK_Token> lex_number();
  optional<BASK_Token> lex_string_literal();
  optional<BASK_Token> lex_char_literal();
  optional<char> let_escape_char();
};

#endif //BASK_LEXER_H
