//
// Created by kiran on 5/21/24.
//

#include "AST/AST.h"
#include "AST/AST_Expressions.h"


unique_ptr<ASTExpr> ASTExpr::fold_unary(ASTExpr* expr) {
  auto unary = dynamic_cast<ASTExprUnary *>(expr);
  unary->expr = fold(unary->expr.get());
  
  // If it not a constant, return the expression as is
  if (unary->expr->get_AST_type() != ASTNode::ASTNodeType::ExprValue) {
    return make_unique<ASTExprUnary>(unary->op, fold(unary->expr.get()), unary->line, unary->col);
  }
  
  auto value = dynamic_cast<ASTExprValue *>(unary->expr.get());
  if (value->type.primitive == KL_INT) {
    int val = stoi(value->value);
    switch (unary->op) {
      case KL_TT_Operator_Sub:
        return make_unique<ASTExprValue>(value->type, to_string(-val), value->line, value->col);
      case KL_TT_Operator_BitwiseNot:
        return make_unique<ASTExprValue>(value->type, to_string(!val), value->line, value->col);
      default:
        ASTNode::SyntaxError(unary, "Invalid unary operator for int value");
    }
  } else if (value->type.primitive == KL_FLOAT) {
    float val = stof(value->value);
    switch (unary->op) {
      case KL_TT_Operator_Sub:
        return make_unique<ASTExprValue>(value->type, to_string(-val), value->line, value->col);
      default:
        ASTNode::SyntaxError(unary, "Invalid unary operator for float value");
    }
  } else if (value->type.primitive == KL_BOOL) {
    bool val = value->value == "true";
    switch (unary->op) {
      case KL_TT_Operator_LogicalNot:
        return make_unique<ASTExprValue>(value->type, val ? "false" : "true", value->line, value->col);
      default:
        ASTNode::SyntaxError(unary, "Invalid unary operator for boolean value");
    }
  }
  
  ASTNode::SyntaxError(unary, "Unary operators are only supported for int, float and bool types");
}

unique_ptr<ASTExpr> ASTExpr::fold(ASTExpr *expr) {
  if (expr->get_AST_type() == ASTNode::ASTNodeType::ExprValue) {
    return make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(expr));
  }
  
  if (expr->get_AST_type() == ASTNode::ASTNodeType::ExprIdentifier) {
    return make_unique<ASTExprIdentifier>(*dynamic_cast<ASTExprIdentifier *>(expr));
  }
  
  if (expr->get_AST_type() == ASTNode::ASTNodeType::ExprFuncCall) {
    auto func_call = dynamic_cast<ASTExprFuncCall *>(expr);
    for (auto &arg: *func_call->args) {
      arg = fold(arg.get());
    }
    return make_unique<ASTExprFuncCall>(func_call->name, std::move(func_call->args), func_call->line, func_call->col);
  }
  
  if (expr->get_AST_type() == ASTNode::ASTNodeType::ExprUnary) {
    return fold_unary(expr);
  }
  
  if (expr->get_AST_type() == ASTNode::ASTNodeType::ExprBinary) {
    return fold_binary(expr);
  }
}