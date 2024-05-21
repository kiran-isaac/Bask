//
// Created by kiran on 5/20/24.
//

#include <parser.h>
#include <memory>

using namespace std;

unique_ptr<ASTExpr> Parser::parsePrimaryParens() {
  nextToken();
  auto expr = parseExpression();
  if (tk.type != KL_TT_Punctuation_RParen) {
    parserError("Unmatched, parsing primary expression");
  }
  nextToken();
  return expr;
}

unique_ptr<ASTExprFuncCall> Parser::parseFunctionCall() {
  string name = tk.value;
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  nextToken();
  unique_ptr<vector<unique_ptr<ASTExpr>>> args = make_unique<vector<unique_ptr<ASTExpr>>>();
  while (tk.type != KL_TT_Punctuation_RParen) {
    nextToken();
    args->push_back(parseExpression());
    if (tk.type == KL_TT_Punctuation_Comma) {
      nextToken();
    }
  }
  nextToken();
  return make_unique<ASTExprFuncCall>(name, std::move(args), line, col);
}

unique_ptr<ASTExpr> Parser::parsePrimary() {
  unique_ptr<ASTExpr> return_primary;
  switch (tk.type) {
    case KL_TT_Literal_Int:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_INT}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KL_TT_Literal_Float:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_FLOAT}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KL_TT_Literal_Bool:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_BOOL}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KL_TT_Literal_Char:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_CHAR}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KL_TT_Literal_String:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_STRING}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
      
    case KL_TT_Identifier:
      if (peek(1).type == KL_TT_Punctuation_LParen) {
        return parseFunctionCall();
      } else if (peek(1).type == KL_TT_Punctuation_LBracket) {
        parserError("Array access not implemented");
        // return parseArrayAccess();
      } else if (peek(1).type == KL_TT_Operator_Assign) {
        parserError("Assignment not implemented");
        // return parseAssignment();
      }
      return_primary = make_unique<ASTExprIdentifier>(tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KL_TT_Punctuation_LParen:
      return parsePrimaryParens();
    default:
      parserError("Expected primary expression");
  }
}

#define IS_UNARY_OP(type) (type == KL_TT_Operator_BitwiseNot || type == KL_TT_Operator_LogicalNot || type == KL_TT_Operator_Sub)
#define IS_BINARY_OP(type) (type == KL_TT_Operator_Add || type == KL_TT_Operator_Sub || type == KL_TT_Operator_Mul || type == KL_TT_Operator_Div || type == KL_TT_Operator_Mod || type == KL_TT_Operator_BitwiseAnd || type == KL_TT_Operator_BitwiseOr || type == KL_TT_Operator_BitwiseXor || type == KL_TT_Operator_LogicalAnd || type == KL_TT_Operator_LogicalOr || type == KL_TT_Operator_Equal || type == KL_TT_Operator_NotEqual || type == KL_TT_Operator_Less || type == KL_TT_Operator_LessEqual || type == KL_TT_Operator_Greater || type == KL_TT_Operator_GreaterEqual || type == KL_TT_Operator_Shl || type == KL_TT_Operator_Shr)

unique_ptr<ASTExpr> Parser::parseUnaryExpression() {
  KL_Token op = tk;
  nextToken();
  return make_unique<ASTExprUnary>(op.type, parsePrimary(), op.line, op.col);
}

unique_ptr<ASTExpr> Parser::parseBinaryExpression(unique_ptr<ASTExpr> LHS, int min_precedence) {
  while (true) {
    int precedence = getOperatorPrecedence(tk.type);
    if (precedence < min_precedence) {
      return LHS;
    }
    
    KL_Token op = tk;
    nextToken();
    
    unique_ptr<ASTExpr> RHS;
    
    if (IS_UNARY_OP(tk.type)) {
      RHS = parseUnaryExpression();
    } else {
      RHS = parsePrimary();
    }
    
    int next_precedence = getOperatorPrecedence(tk.type);
    if (precedence < next_precedence) {
      RHS = parseBinaryExpression(std::move(RHS), precedence + 1);
    }
    
    LHS = make_unique<ASTExprBinary>(std::move(LHS), std::move(RHS), op.type, op.line, op.col);
  }
}

unique_ptr<ASTExpr> Parser::parseExpression() {
  if (IS_UNARY_OP(tk.type)) {
    return parseBinaryExpression(std::move(parseUnaryExpression()), 0);
  } else {
    return parseBinaryExpression(std::move(parsePrimary()), 0);
  }
}