//
// Created by kiran on 5/20/24.
//

#include "tokens.h"
#include <parser.h>
#include <iostream>

using namespace std;

void Parser::expect(BASK_TokenType type) const {
  if (tk.type != type) {
    parserError("Expected " + string(token_type_to_string(type)) + " but got " + string(token_type_to_string(tk.type)));
  }
}

void Parser::nextToken() {
  if (!peekQueue.empty()) {
    tk = peekQueue.front();
    peekQueue.pop();
    return;
  }
  auto lexer_returns = lexer.next();
  if (lexer_returns.has_value()) {
    tk = lexer_returns.value();
  } else {
    parserError("Unexpected end of file");
  }
}

// IDE was giving me a warning about the loop not using the condition variable
// This is a false positive, as the queue is getting longer with each iteration
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
BASK_Token Parser::peek(int n) {
  while (n > peekQueue.size()) {
    peekQueue.push(lexer.next().value());
  }
  
  return peekQueue.front();
}
#pragma clang diagnostic pop

void Parser::parserError(const string &msg) const {
  cerr << "Parser error at line " << tk.line << ", col " << tk.col << ": " << msg << endl;
  exit(1);
}

unique_ptr<ASTType> Parser::parse_type_annotation() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  BASK_Type type;
  
  if (tk.type == BASK_TT_KW_Const) {
    type.is_const = true;
    nextToken();
  }
  
  type.kind = BASK_PRIMITIVE_TYPEKIND;
  switch (tk.type) {
    case BASK_TT_KW_Int:
      type.primitive = BASK_INT_PRIMITIVE;
      break;
    case BASK_TT_KW_Float:
      type.primitive = BASK_FLOAT_PRIMITIVE;
      break;
    case BASK_TT_KW_Bool:
      type.primitive = BASK_BOOL_PRIMITIVE;
      break;
    case BASK_TT_KW_Char:
      type.primitive = BASK_CHAR_PRIMITIVE;
      break;
    case BASK_TT_KW_String:
      type.primitive = BASK_STRING_PRIMITIVE;
      break;
    default:
      parserError("Expected type");
  }
  
  nextToken();
  while (tk.type == BASK_TT_Punctuation_LBracket) {
    nextToken();
    type.kind = BASK_ARRAY_TYPEKIND;
    type.array_sizes = new vector<unsigned int>();
    switch (tk.type) {
      case BASK_TT_Literal_Int:
        type.array_sizes->push_back(stoi(tk.value));
        nextToken();
        if (tk.type != BASK_TT_Punctuation_RBracket) {
          parserError("Expected ']' after integer in array declaration");
        }
      case BASK_TT_Punctuation_RBracket:
        type.array_sizes->push_back(0);
        nextToken();
        break;
      default:
        parserError("Expected integer");
    }
  }
  
  return make_unique<ASTType>(type, line, col);
}

unique_ptr<ASTBlock> Parser::parse_block() {
  expect(BASK_TT_Punctuation_LBrace);
  nextToken();
  
  vector<unique_ptr<ASTStmt>> body;
  while (tk.type != BASK_TT_Punctuation_RBrace) {
    body.push_back(parse_statement());
  }
  
  nextToken();
  
  return make_unique<ASTBlock>(std::move(body), tk.line, tk.col);
}

unique_ptr<ASTFuncDecl> Parser::parse_function() {
  unique_ptr<ASTType> return_type = parse_type_annotation();
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  if (tk.type != BASK_TT_Identifier) {
    parserError("Expected identifier");
  }
  
  string name = tk.value;
  nextToken();
  
  expect(BASK_TT_Punctuation_LParen);
  nextToken();
  
  vector<unique_ptr<ASTType>> arg_types;
  vector<string> arg_names;
  
  while (tk.type != BASK_TT_Punctuation_RParen) {
    arg_types.push_back(parse_type_annotation());
    expect(BASK_TT_Identifier);
    arg_names.push_back(tk.value);
    nextToken();
    if (tk.type == BASK_TT_Punctuation_Comma) {
      nextToken();
    }
  }
  
  nextToken();

  if (tk.type == BASK_TT_Punctuation_LBrace) {
    return std::make_unique<ASTFuncDecl>(
        name, std::move(return_type), std::move(arg_types),
        std::move(arg_names), std::move(parse_block()), line, col);
  } else if (tk.type == BASK_TT_Punctuation_Semicolon) {
    nextToken();
    return std::make_unique<ASTFuncDecl>(name, std::move(return_type), std::move(arg_types), std::move(arg_names), nullptr, line, col);
  } else {
    parserError("Unexpected token after function declaration: Expected '{' or ';'");
  }
}

unique_ptr<ASTProgram> Parser::parse() {
  auto program = make_unique<ASTProgram>();

  while (tk.type != BASK_TT_EndOfFile) {
    if (tk.type == BASK_TT_KW_Use) {
      nextToken();
      if (tk.type != BASK_TT_Literal_String) {
        parserError("Expected string after 'use'");
      }
      string module = lexer.options.find_module(tk.value);
      nextToken();
      if (module.empty()) {
        parserError("Module not found: " + tk.value);
      }
      Lexer moduleLexer(module, lexer.options);
      Parser moduleParser(moduleLexer);
      auto moduleAST = moduleParser.parse();
      program->prepend(moduleAST.get());
      expect(BASK_TT_Punctuation_Semicolon);
      nextToken();
    } else {
      auto func = parse_function();
      auto name = string(func->name);
      if (!program->add_function(std::move(func))) {
        parserError("Function redefined: " + name);
      }
    }
  }

  return program;
}