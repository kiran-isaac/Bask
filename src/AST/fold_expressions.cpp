//
// Created by kiran on 5/21/24.
//

#include <AST.h>
#include <cmath>


unique_ptr<ASTExpr> ASTExpr::foldUnary(ASTExpr* expr) {
  auto unary = dynamic_cast<ASTExprUnary *>(expr);
  unary->expr = fold(unary->expr.get());
  
  // If it not a constant, return the expression as is
  if (unary->expr->getAstType() != ASTNode::ASTNodeType::ExprValue) {
    return make_unique<ASTExprUnary>(unary->op, fold(unary->expr.get()), unary->line, unary->col);
  }
  
  auto value = dynamic_cast<ASTExprValue *>(unary->expr.get());
  if (value->type.primitive == KL_INT) {
    int val = stoi(value->value);
    switch (unary->op) {
      case KLTT_Operator_Sub:
        return make_unique<ASTExprValue>(value->type, to_string(-val), value->line, value->col);
      case KLTT_Operator_BitwiseNot:
        return make_unique<ASTExprValue>(value->type, to_string(!val), value->line, value->col);
      default:
        ASTNode::SyntaxError(unary, "Invalid unary operator for int value");
    }
  } else if (value->type.primitive == KL_FLOAT) {
    float val = stof(value->value);
    switch (unary->op) {
      case KLTT_Operator_Sub:
        return make_unique<ASTExprValue>(value->type, to_string(-val), value->line, value->col);
      default:
        ASTNode::SyntaxError(unary, "Invalid unary operator for float value");
    }
  } else if (value->type.primitive == KL_BOOL) {
    bool val = value->value == "true";
    switch (unary->op) {
      case KLTT_Operator_LogicalNot:
        return make_unique<ASTExprValue>(value->type, val ? "false" : "true", value->line, value->col);
      default:
        ASTNode::SyntaxError(unary, "Invalid unary operator for boolean value");
    }
  }
  
  ASTNode::SyntaxError(unary, "Unary operators are only supported for int, float and bool types");
}

unique_ptr<ASTExpr> ASTExpr::foldBinary(ASTExpr *expr) {
  auto binary = dynamic_cast<ASTExprBinary *>(expr);
  binary->lhs = fold(binary->lhs.get());
  binary->rhs = fold(binary->rhs.get());
  
  // TODO: Implement constant folding where commutative but not in right order
  
  // If both are not constant, return the expression as is
  if (binary->lhs->getAstType() != ASTNode::ASTNodeType::ExprValue || binary->rhs->getAstType() != ASTNode::ASTNodeType::ExprValue) {
    return make_unique<ASTExprBinary>(fold(binary->lhs.get()), fold(binary->rhs.get()), binary->op, binary->line, binary->col);
  }
  
  auto lhs = dynamic_cast<ASTExprValue *>(binary->lhs.get());
  auto rhs = dynamic_cast<ASTExprValue *>(binary->rhs.get());
  
  if (!lhs || !rhs || lhs->type.kind != KL_PRIMITIVE || rhs->type.kind != KL_PRIMITIVE) {
    throw std::runtime_error("Binary expression does not have constant values");
  }
  
  if (lhs->type.primitive == KL_STRING || rhs->type.primitive == KL_STRING) {
    // v1 is definitely a string
    auto v1 = lhs->type.primitive == KL_STRING ? lhs : rhs;
    string s1 = v1->value;
    
    // v2 could be a string, a char
    auto v2 = lhs->type.primitive == KL_STRING ? rhs : lhs;
    string s2;
    
    switch (v2->type.primitive) {
      default:
        ASTNode::SyntaxError(binary, "Invalid type in string expression");
      case KL_STRING:
      case KL_CHAR:
        s2 = v2->value;
    }
    
    switch (binary->op) {
      case KLTT_Operator_Add:
        return make_unique<ASTExprValue>(v1->type, s1 + s2, v1->line, v1->col);
      case KLTT_Operator_Equal:
        return make_unique<ASTExprValue>(BOOL_CONST, s1 == s2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_NotEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, s1 != s2 ? "true" : "false", v1->line, v1->col);
      default:
        ASTNode::SyntaxError(binary, "Invalid operator in string expression");
    }
  } else if (lhs->type.primitive == KL_FLOAT || rhs->type.primitive == KL_FLOAT) {
    // v1 is definitely a float
    auto v1 = lhs->type.primitive == KL_FLOAT ? lhs : rhs;
    
    // v2 could be a float, an int, or a char
    auto v2 = lhs->type.primitive == KL_FLOAT ? rhs : lhs;
    
    float f1 = stof(v1->value);
    
    float f2;
    
    switch (v2->type.primitive) {
      default:
        ASTNode::SyntaxError(binary, "Invalid type in float expression");
      case KL_FLOAT:
      case KL_INT:
        f2 = stof(v2->value);
        break;
      case KL_CHAR:
        f2 = v2->value[0];
    }
    
    switch (binary->op) {
      case KLTT_Operator_Add:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 + f2), v1->line, v1->col);
      case KLTT_Operator_Sub:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 - f2), v1->line, v1->col);
      case KLTT_Operator_Mul:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 * f2), v1->line, v1->col);
      case KLTT_Operator_Div:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 / f2), v1->line, v1->col);
      case KLTT_Operator_Mod:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(fmod(f1, f2)), v1->line, v1->col);
      case KLTT_Operator_Equal:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 == f2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_NotEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 != f2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_Less:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 < f2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_LessEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 <= f2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_Greater:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 > f2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_GreaterEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 >= f2 ? "true" : "false", v1->line, v1->col);
      default:
        ASTNode::SyntaxError(binary, "Invalid operator in float expression");
    }
  } else if (lhs->type.primitive == KL_INT || rhs->type.primitive == KL_INT) {
    // v1 is definitely an int
    auto v1 = (lhs->type.primitive == KL_INT) ? lhs : rhs;
    
    auto v2 = (lhs->type.primitive == KL_INT) ? rhs : lhs;
    
    int i1 = stoi(v1->value);
    
    int i2;
    
    switch (v2->type.primitive) {
      default:
        ASTNode::SyntaxError(binary, "Invalid type in int expression");
      case KL_INT:
        i2 = stoi(v2->value);
        break;
      case KL_CHAR:
        i2 = (unsigned char) v2->value[0];
    }
    
    switch (binary->op) {
      case KLTT_Operator_Add:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 + i2), v1->line, v1->col);
      case KLTT_Operator_Sub:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 - i2), v1->line, v1->col);
      case KLTT_Operator_Mul:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 * i2), v1->line, v1->col);
      case KLTT_Operator_Div:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 / i2), v1->line, v1->col);
      case KLTT_Operator_Mod:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 % i2), v1->line, v1->col);
      case KLTT_Operator_Equal:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 == i2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_NotEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 != i2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_Less:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 < i2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_LessEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 <= i2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_Greater:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 > i2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_GreaterEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 >= i2 ? "true" : "false", v1->line, v1->col);
      case KLTT_Operator_Shl:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 << i2), v1->line, v1->col);
      case KLTT_Operator_Shr:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 >> i2), v1->line, v1->col);
      case KLTT_Operator_BitwiseAnd:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 & i2), v1->line, v1->col);
      case KLTT_Operator_BitwiseOr:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 | i2), v1->line, v1->col);
      case KLTT_Operator_BitwiseXor:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 ^ i2), v1->line, v1->col);
      default:
        ASTNode::SyntaxError(binary, "Invalid operator in int expression");
    }
  }
  ASTNode::SyntaxError(binary, "Binary expressions are only supported for int, float and string types");
  return nullptr;
}

unique_ptr<ASTExpr> ASTExpr::fold(ASTExpr *expr) {
  if (expr->getAstType() == ASTNode::ASTNodeType::ExprValue) {
    return make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(expr));
  }
  
  if (expr->getAstType() == ASTNode::ASTNodeType::ExprIdentifier) {
    return make_unique<ASTExprIdentifier>(*dynamic_cast<ASTExprIdentifier *>(expr));
  }
  
  if (expr->getAstType() == ASTNode::ASTNodeType::ExprFuncCall) {
    auto func_call = dynamic_cast<ASTExprFuncCall *>(expr);
    for (auto &arg: *func_call->args) {
      arg = fold(arg.get());
    }
    return make_unique<ASTExprFuncCall>(func_call->name, std::move(func_call->args), func_call->line, func_call->col);
  }
  
  if (expr->getAstType() == ASTNode::ASTNodeType::ExprUnary) {
    return foldUnary(expr);
  }
  
  if (expr->getAstType() == ASTNode::ASTNodeType::ExprBinary) {
    return foldBinary(expr);
  }
}