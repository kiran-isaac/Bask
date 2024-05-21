//
// Created by kiran on 5/20/24.
//

#include <parser.h>

using namespace std;

unique_ptr<ASTStmtDecl> Parser::parseDeclaration() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  auto type= parseType();
  expect(KL_TT_Identifier);
  auto name = tk.value;
  nextToken();
  expect(KL_TT_Operator_Assign);
  nextToken();
  auto value= parseExpression();
  
  expect(KL_TT_Punctuation_Semicolon);
  nextToken();
  
  return make_unique<ASTStmtDecl>(std::move(type), name, std::move(value), line, col);
}

unique_ptr<ASTStmtAssignment> Parser::parseAssignment() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  auto name = tk.value;
  nextToken();
  expect(KL_TT_Operator_Assign);
  nextToken();
  auto value = parseExpression();
  
  expect(KL_TT_Punctuation_Semicolon);
  nextToken();
  
  return make_unique<ASTStmtAssignment>(name, std::move(value), line, col);

}

unique_ptr<ASTStmtExpr> Parser::parseExpressionStatement() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  auto expr = parseExpression();
  expect(KL_TT_Punctuation_Semicolon);
  nextToken();
  return make_unique<ASTStmtExpr>(std::move(expr), line, col);
}

unique_ptr<ASTStmt> Parser::parseStatement() {
  switch (tk.type) {
    case KL_TT_KW_Const:
    case KL_TT_KW_Int:
    case KL_TT_KW_Float:
    case KL_TT_KW_Bool:
    case KL_TT_KW_Char:
    case KL_TT_KW_String:
      return parseDeclaration();
    case KL_TT_Identifier:
      switch (peek(1).type) {
        case KL_TT_Punctuation_LParen:
          return parseExpressionStatement();
        case KL_TT_Operator_Assign:
          return parseAssignment();
      }
    default:
      parserError("Expected assignment");
  }
}