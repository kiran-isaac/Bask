//
// Created by kiran on 5/20/24.
//

#include <parser.h>
#include <iostream>

using namespace std;

void Parser::expect(KL_TokenType type) const {
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
KL_Token Parser::peek(int n) {
  while (n > peekQueue.size()) {
    peekQueue.push(lexer.next().value());
  }
  
  return peekQueue.front();
}
#pragma clang diagnostic pop

void Parser::parserError(const string &msg) {
  cerr << msg << endl;
  exit(1);
}

unique_ptr<ASTType> Parser::parse_type_annotation() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  KL_Type type;
  
  if (tk.type == KL_TT_KW_Const) {
    type.is_const = true;
    nextToken();
  }
  
  type.kind = KL_PRIMITIVE;
  switch (tk.type) {
    case KL_TT_KW_Int:
      type.primitive = KL_INT;
      break;
    case KL_TT_KW_Float:
      type.primitive = KL_FLOAT;
      break;
    case KL_TT_KW_Bool:
      type.primitive = KL_BOOL;
      break;
    case KL_TT_KW_Char:
      type.primitive = KL_CHAR;
      break;
    case KL_TT_KW_String:
      type.primitive = KL_STRING;
      break;
    default:
      parserError("Expected type");
  }
  
  nextToken();
  while (tk.type == KL_TT_Punctuation_LBracket) {
    nextToken();
    type.kind = KL_ARRAY;
    switch (tk.type) {
      case KL_TT_Literal_Int:
        type.array_sizes.push_back(stoi(tk.value));
        nextToken();
        if (tk.type != KL_TT_Punctuation_RBracket) {
          parserError("Expected ']' after integer in array declaration");
        }
      case KL_TT_Punctuation_RBracket:
        type.array_sizes.push_back(0);
        nextToken();
        break;
      default:
        parserError("Expected integer");
    }
  }
  
  return make_unique<ASTType>(type, line, col);
}

unique_ptr<ASTBlock> Parser::parse_block() {
  expect(KL_TT_Punctuation_LBrace);
  nextToken();
  
  vector<unique_ptr<ASTStmt>> body;
  while (tk.type != KL_TT_Punctuation_RBrace) {
    body.push_back(parse_statement());
  }
  
  nextToken();
  
  return make_unique<ASTBlock>(std::move(body), tk.line, tk.col);
}

unique_ptr<ASTFuncDecl> Parser::parse_function() {
  unique_ptr<ASTType> return_type = parse_type_annotation();
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  if (tk.type != KL_TT_Identifier) {
    parserError("Expected identifier");
  }
  
  string name = tk.value;
  nextToken();
  
  expect(KL_TT_Punctuation_LParen);
  nextToken();
  
  vector<unique_ptr<ASTType>> arg_types;
  vector<string> arg_names;
  
  while (tk.type != KL_TT_Punctuation_RParen) {
    arg_types.push_back(parse_type_annotation());
    expect(KL_TT_Identifier);
    arg_names.push_back(tk.value);
    nextToken();
    if (tk.type == KL_TT_Punctuation_Comma) {
      nextToken();
    }
  }
  
  nextToken();
  
  return std::make_unique<ASTFuncDecl>(name, std::move(return_type), std::move(arg_types), std::move(arg_names), std::move(parse_block()), line, col);
}

unique_ptr<ASTProgram> Parser::parse() {
  vector<unique_ptr<ASTFuncDecl>> functions;
  
  while (tk.type != KL_TT_EndOfFile) {
    functions.push_back(parse_function());
  }
  
  return make_unique<ASTProgram>(std::move(functions));
}