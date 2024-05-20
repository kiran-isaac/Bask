//
// Created by kiran on 5/20/24.
//

#include <parser.h>

using namespace std;

unique_ptr<ASTStmt> Parser::parseAssignment() {
  bool is_const = false;
  if (tk.type == KLTT_KW_Const) {
    is_const = true;
    nextToken();
  }
  
  auto type= parseType();
  expect(KLTT_Identifier);
  auto name = tk.value;
  nextToken();
  expect(KLTT_Operator_Assign);
  nextToken();
  auto value = parseExpression();
  
  return make_unique<ASTStmtAssignment>(is_const, std::move(type), name, std::move(value));
}

unique_ptr<ASTStmtExpr> Parser::parseExpressionStatement() {
  auto expr = parseExpression();
  expect(KLTT_Punctuation_Semicolon);
  nextToken();
  return make_unique<ASTStmtExpr>(std::move(expr));
}

unique_ptr<ASTStmt> Parser::parseStatement() {
  switch (tk.type) {
    case KLTT_KW_Const:
    case KLTT_KW_Int:
    case KLTT_KW_Float:
    case KLTT_KW_Bool:
    case KLTT_KW_Char:
    case KLTT_KW_String:
      return parseAssignment();
    case KLTT_Identifier:
      return parseExpressionStatement();
    default:
      parserError("Expected assignment");
  }
}