//
// Created by kiran on 5/20/24.
//

#include <parser.h>
#include <memory>

using namespace std;

unique_ptr<ASTExpr> Parser::parsePrimaryParens() {
  nextToken();
  auto expr = parseExpression();
  if (tk.type != KLTT_Punctuation_RParen) {
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
  while (tk.type != KLTT_Punctuation_RParen) {
    nextToken();
    args->push_back(parseExpression());
    if (tk.type == KLTT_Punctuation_Comma) {
      nextToken();
    }
  }
  nextToken();
  return make_unique<ASTExprFuncCall>(name, std::move(args), line, col);
}

unique_ptr<ASTExpr> Parser::parsePrimary() {
  unique_ptr<ASTExpr> return_primary;
  switch (tk.type) {
    case KLTT_Literal_Int:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_INT}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KLTT_Literal_Float:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_FLOAT}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KLTT_Literal_Bool:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_BOOL}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KLTT_Literal_Char:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_CHAR}, tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KLTT_Literal_String:
      return_primary = make_unique<ASTExprValue>(KL_Type{true, KL_PRIMITIVE, KL_STRING}, tk.value, tk.line, tk.col);
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
      return_primary = make_unique<ASTExprIdentifier>(tk.value, tk.line, tk.col);
      nextToken();
      return return_primary;
    case KLTT_Punctuation_LParen:
      return parsePrimaryParens();
    default:
      parserError("Expected primary expression");
  }
}

int getOperatorPrecedence(KLTokenType type) {
  switch (type) {
    case KLTT_Operator_Assign:
      return 1;
    case KLTT_Operator_LogicalOr:
      return 2;
    case KLTT_Operator_LogicalAnd:
      return 3;
    case KLTT_Operator_BitwiseOr:
      return 4;
    case KLTT_Operator_BitwiseXor:
      return 5;
    case KLTT_Operator_BitwiseAnd:
      return 6;
    case KLTT_Operator_Equal:
    case KLTT_Operator_NotEqual:
      return 7;
    case KLTT_Operator_Less:
    case KLTT_Operator_LessEqual:
    case KLTT_Operator_Greater:
    case KLTT_Operator_GreaterEqual:
      return 8;
    case KLTT_Operator_Shl:
    case KLTT_Operator_Shr:
      return 9;
    case KLTT_Operator_Add:
    case KLTT_Operator_Sub:
      return 10;
    case KLTT_Operator_Mul:
    case KLTT_Operator_Div:
    case KLTT_Operator_Mod:
      return 11;
    case KLTT_Operator_BitwiseNot:
    case KLTT_Operator_LogicalNot:
      return 12;
    default:
      return -1;
  }
}

#define IS_UNARY_OP(type) (type == KLTT_Operator_BitwiseNot || type == KLTT_Operator_LogicalNot || type == KLTT_Operator_Sub)
#define IS_BINARY_OP(type) (type == KLTT_Operator_Add || type == KLTT_Operator_Sub || type == KLTT_Operator_Mul || type == KLTT_Operator_Div || type == KLTT_Operator_Mod || type == KLTT_Operator_BitwiseAnd || type == KLTT_Operator_BitwiseOr || type == KLTT_Operator_BitwiseXor || type == KLTT_Operator_LogicalAnd || type == KLTT_Operator_LogicalOr || type == KLTT_Operator_Equal || type == KLTT_Operator_NotEqual || type == KLTT_Operator_Less || type == KLTT_Operator_LessEqual || type == KLTT_Operator_Greater || type == KLTT_Operator_GreaterEqual || type == KLTT_Operator_Shl || type == KLTT_Operator_Shr)

unique_ptr<ASTExpr> Parser::parseUnaryExpression() {
  Token op = tk;
  nextToken();
  return make_unique<ASTExprUnary>(op.type, parsePrimary(), op.line, op.col);
}

unique_ptr<ASTExpr> Parser::parseBinaryExpression(unique_ptr<ASTExpr> LHS, int min_precedence) {
  while (true) {
    int precedence = getOperatorPrecedence(tk.type);
    if (precedence < min_precedence) {
      return LHS;
    }
    
    Token op = tk;
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