//
// Created by kiran on 5/20/24.
//

#include "tokens.h"
#include <memory>
#include <parser.h>

using namespace std;

unique_ptr<ASTStmtDecl> Parser::parse_declaration() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  auto type= parse_type_annotation();
  expect(BASK_TT_Identifier);
  auto name = tk.value;
  nextToken();
  expect(BASK_TT_Operator_Assign);
  nextToken();
  auto value= parse_expression();
  
  expect(BASK_TT_Punctuation_Semicolon);
  nextToken();
  
  return make_unique<ASTStmtDecl>(std::move(type), name, std::move(value), line, col);
}

unique_ptr<ASTStmtAssignment> Parser::parse_assignment() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  auto name = tk.value;
  nextToken();
  expect(BASK_TT_Operator_Assign);
  nextToken();
  auto value = parse_expression();
  
  expect(BASK_TT_Punctuation_Semicolon);
  nextToken();
  
  return make_unique<ASTStmtAssignment>(name, std::move(value), line, col);
}

unique_ptr<ASTStmtExpr> Parser::parse_expression_statement() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  auto expr = parse_expression();
  expect(BASK_TT_Punctuation_Semicolon);
  nextToken();
  return make_unique<ASTStmtExpr>(std::move(expr), line, col);
}

unique_ptr<ASTStmtReturn> Parser::parse_return() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  nextToken();
  auto expr = parse_expression();
  expect(BASK_TT_Punctuation_Semicolon);
  nextToken();
  return make_unique<ASTStmtReturn>(std::move(expr), line, col);
}

unique_ptr<ASTStmt> Parser::parse_statement() {
  switch (tk.type) {
  case BASK_TT_KW_Return:
    return parse_return();
  
  case BASK_TT_KW_If:
    return parse_if();

  case BASK_TT_KW_While:
    return parse_while();

  case BASK_TT_KW_Const:
  case BASK_TT_KW_Int:
  case BASK_TT_KW_Float:
  case BASK_TT_KW_Bool:
  case BASK_TT_KW_Char:
  case BASK_TT_KW_String:
    return parse_declaration();

  // if identifier then check if it is a function call, an assignment or an
  // expression statement
  case BASK_TT_Identifier:
    switch (peek(1).type) {
    // Function call
    case BASK_TT_Punctuation_LParen:
    // Or binary operator
    case BIN_OP_CASES:
      return parse_expression_statement();

    case BASK_TT_Operator_Assign:
      return parse_assignment();

    default:
      parserError("Invalid token after identifier");
    }

    // if it is a literal or a unary operator then it is an expression statement
    case LITERAL_CASES:
    case UN_OP_CASES:
      return parse_expression_statement();

    
    default:
      parserError("Invalid token to start a statement");
  }
}