//
// Created by kiran on 5/20/24.
//

#include <parser.h>
#include <memory>

using namespace std;

unique_ptr<ASTExprParen> Parser::parsePrimaryParens() {
  nextToken();
  auto expr = parseExpression();
  if (tk.type != KLTT_Punctuation_RParen) {
    parserError("Unmatched, parsing primary expression");
  }
  nextToken();
  return make_unique<ASTExprParen>(std::move(expr));
}

unique_ptr<ASTExprFuncCall> Parser::parseFunctionCall() {
  string name = tk.value;
  
  nextToken();
  unique_ptr<vector<unique_ptr<ASTExpr>>> args = make_unique<vector<unique_ptr<ASTExpr>>>();
  while (tk.type != KLTT_Punctuation_RParen) {
    nextToken();
    args->push_back(parseExpression());
    if (tk.type == KLTT_Punctuation_Comma) {
      nextToken();
    }
  }
  nextToken();
  return make_unique<ASTExprFuncCall>(name, std::move(args));
}

unique_ptr<ASTExprPrimary> Parser::parsePrimary() {
  unique_ptr<ASTExprPrimary> return_primary;
  switch (tk.type) {
    case KLTT_Literal_Int:
    case KLTT_Literal_Float:
    case KLTT_Literal_Bool:
    case KLTT_Literal_Char:
    case KLTT_Literal_String:
      return_primary = make_unique<ASTExprLiteral>(tk.type, tk.value);
      nextToken();
      return return_primary;
    case KLTT_Identifier:
      if (peek(1).type == KLTT_Punctuation_LParen) {
        return parseFunctionCall();
      } else if (peek(1).type == KLTT_Punctuation_LBracket) {
        parserError("Array access not implemented");
        // return parseArrayAccess();
      } else if (peek(1).type == KLTT_Operator_Assign) {
        parserError("Assignment not implemented");
        // return parseAssignment();
      }
      return make_unique<ASTExprIdentifier>(tk.value);
    case KLTT_Punctuation_LParen:
      return parsePrimaryParens();
      
    default:
      parserError("Expected primary expression");
  }
}

unique_ptr<ASTExpr> Parser::parseExpression() {
  // Check for unary operators
  switch (tk.type) {
    case KLTT_Operator_Add:
    case KLTT_Operator_Sub:
    case KLTT_Operator_BitwiseNot:
    case KLTT_Operator_LogicalNot:
      nextToken();
      return make_unique<ASTExprUnary>(tk.type, parsePrimary());
    default:
      return parsePrimary();
  }
}