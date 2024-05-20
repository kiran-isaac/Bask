//
// Created by kiran on 5/20/24.
//

#include <parser.h>
#include <iostream>

using namespace std;

void Parser::expect(KLTokenType type) const {
  if (tk.type != type) {
    parserError("Expected " + string(tokenTypeToString(type)) + " but got " + string(tokenTypeToString(tk.type)));
  }
}

void Parser::nextToken() {
  auto lexer_returns = lexer.next();
  if (lexer_returns.has_value()) {
    tk = lexer_returns.value();
  } else {
    parserError("Unexpected end of file");
  }
}

void Parser::parserError(const string &msg) {
  cerr << msg << endl;
  exit(1);
}

unique_ptr<ASTType> Parser::parseType() {
  KL_Type type;
  
  if (tk.type == KLTT_KW_Const) {
    type.is_const = true;
    nextToken();
  }
  
  type.kind = KL_PRIMITIVE;
  switch (tk.type) {
    case KLTT_KW_Int:
      type.primitive = KL_INT;
      break;
    case KLTT_KW_Float:
      type.primitive = KL_FLOAT;
      break;
    case KLTT_KW_Bool:
      type.primitive = KL_BOOL;
      break;
    case KLTT_KW_Char:
      type.primitive = KL_CHAR;
      break;
    case KLTT_KW_String:
      type.primitive = KL_STRING;
      break;
    default:
      parserError("Expected type");
  }
  
  nextToken();
  while (tk.type == KLTT_Punctuation_LBracket) {
    nextToken();
    type.kind = KL_ARRAY;
    switch (tk.type) {
      case KLTT_Literal_Integer:
        type.array_sizes.push_back(stoi(tk.value));
        nextToken();
        if (tk.type != KLTT_Punctuation_RBracket) {
          parserError("Expected ']' after integer in array declaration");
        }
      case KLTT_Punctuation_RBracket:
        nextToken();
        break;
      default:
        parserError("Expected integer");
    }
  }
  
  return make_unique<ASTType>(type);
}

unique_ptr<ASTFuncDecl> Parser::parseFunction() {
  unique_ptr<ASTType> return_type = parseType();
  
  if (tk.type != KLTT_Identifier) {
    parserError("Expected identifier");
  }
  
  string name = tk.value;
  nextToken();
  
  expect(KLTT_Punctuation_LParen);
  nextToken();
  expect(KLTT_Punctuation_RParen);
  nextToken();
  
  return std::make_unique<ASTFuncDecl>(name, std::move(return_type), vector<unique_ptr<ASTStmt>>());
}

unique_ptr<ASTNode> Parser::parse() {
  nextToken();
  return parseFunction();
}