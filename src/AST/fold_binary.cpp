//
// Created by kiran on 5/21/24.
//

#include <AST.h>
#include <cmath>

vector<ASTExpr *> getChildrenAndGrandChildren(ASTExprBinary *expr) {
  vector<ASTExpr *> children;
  
  if (expr->rhs->getAstType() == ASTNode::ExprBinary) {
    auto rhs = dynamic_cast<ASTExprBinary *>(expr->rhs.get());
    children.push_back(rhs->lhs.get());
    children.push_back(rhs->rhs.get());
  } else {
    children.push_back(expr->rhs.get());
  }
  
  if (expr->lhs->getAstType() == ASTNode::ExprBinary) {
    auto lhs = dynamic_cast<ASTExprBinary *>(expr->lhs.get());
    children.push_back(lhs->lhs.get());
    children.push_back(lhs->rhs.get());
  } else {
    children.push_back(expr->lhs.get());
  }
  
  return children;
}

bool childrenHaveSameBinaryOpIfNotValues(ASTExprBinary *expr) {
  auto op = expr->op;
  
  if (expr->lhs->getAstType() == ASTNode::ExprBinary) {
    auto lhs = dynamic_cast<ASTExprBinary *>(expr->lhs.get());
    if (lhs->op != op) {
      return false;
    }
  }
  if (expr->rhs->getAstType() == ASTNode::ExprBinary) {
    auto rhs = dynamic_cast<ASTExprBinary *>(expr->rhs.get());
    if (rhs->op != op) {
      return false;
    }
  }
  
  return true;
}

unique_ptr<ASTExprBinary> constructBinaryExpressionFromListAndOp(vector<ASTExpr *> &children, KL_TokenType op) {
  if (children.size() == 1) {
    return make_unique<ASTExprBinary>(make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(children[0])), make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(children[1])), op, line, col);
  }
  
  auto new_lhs = make_unique<ASTExprBinary>(make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(children[0])), make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(children[1])), op, line, col);
  for (int i = 2; i < children.size(); i++) {
    new_lhs = make_unique<ASTExprBinary>(std::move(new_lhs), make_unique<ASTExprValue>(*dynamic_cast<ASTExprValue *>(children[i])), op, line, col);
  }
  return new_lhs;
}

unique_ptr<ASTExpr> ASTExpr::foldBinary(ASTExpr *expr) {
  auto binary = dynamic_cast<ASTExprBinary *>(expr);
  binary->lhs = fold(binary->lhs.get());
  binary->rhs = fold(binary->rhs.get());
  
  if (childrenHaveSameBinaryOpIfNotValues(binary)) {
    auto children = getChildrenAndGrandChildren(binary);
    vector<ASTExpr *> constants;
    vector<ASTExpr *> non_constants;
    
    for (auto &child: children) {
      if (child->getAstType() == ASTNode::ExprValue) {
        constants.push_back(child);
      } else {
        non_constants.push_back(child);
      }
    }
    
    // Put all constants on one side of a binary expression. Dont evaluate them
    if (constants.size() > 1) {
      auto new_lhs = make_unique<ASTExprBinary>(std::move(constants[0]), std::move(constants[1]), binary->op, binary->line, binary->col);
      for (int i = 2; i < constants.size(); i++) {
        new_lhs = make_unique<ASTExprBinary>(std::move(new_lhs), std::move(constants[i]), binary->op, binary->line, binary->col);
      }
      if (non_constants.size() > 0) {
        return make_unique<ASTExprBinary>(std::move(new_lhs), std::move(non_constants[0]), binary->op, binary->line, binary->col);
      }
      return new_lhs;
    }
  }
  
  // Optimisation: Extract as many constants as possible. At the moment expressions aren't re-arranged if the operator is commutative
  if (binary->lhs->getAstType() != ASTNode::ASTNodeType::ExprValue || binary->rhs->getAstType() != ASTNode::ASTNodeType::ExprValue) {
    return make_unique<ASTExprBinary>(std::move(binary->lhs), std::move(binary->rhs), binary->op, binary->line, binary->col);
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
      case KL_TT_Operator_Add:
        return make_unique<ASTExprValue>(v1->type, s1 + s2, v1->line, v1->col);
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprValue>(BOOL_CONST, s1 == s2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_NotEqual:
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
      case KL_TT_Operator_Add:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 + f2), v1->line, v1->col);
      case KL_TT_Operator_Sub:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 - f2), v1->line, v1->col);
      case KL_TT_Operator_Mul:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 * f2), v1->line, v1->col);
      case KL_TT_Operator_Div:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(f1 / f2), v1->line, v1->col);
      case KL_TT_Operator_Mod:
        return make_unique<ASTExprValue>(FLOAT_CONST, to_string(fmod(f1, f2)), v1->line, v1->col);
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 == f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 != f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Less:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 < f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_LessEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 <= f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Greater:
        return make_unique<ASTExprValue>(BOOL_CONST, f1 > f2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_GreaterEqual:
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
      case KL_TT_Operator_Add:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 + i2), v1->line, v1->col);
      case KL_TT_Operator_Sub:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 - i2), v1->line, v1->col);
      case KL_TT_Operator_Mul:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 * i2), v1->line, v1->col);
      case KL_TT_Operator_Div:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 / i2), v1->line, v1->col);
      case KL_TT_Operator_Mod:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 % i2), v1->line, v1->col);
      case KL_TT_Operator_Equal:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 == i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_NotEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 != i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Less:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 < i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_LessEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 <= i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Greater:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 > i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_GreaterEqual:
        return make_unique<ASTExprValue>(BOOL_CONST, i1 >= i2 ? "true" : "false", v1->line, v1->col);
      case KL_TT_Operator_Shl:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 << i2), v1->line, v1->col);
      case KL_TT_Operator_Shr:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 >> i2), v1->line, v1->col);
      case KL_TT_Operator_BitwiseAnd:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 & i2), v1->line, v1->col);
      case KL_TT_Operator_BitwiseOr:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 | i2), v1->line, v1->col);
      case KL_TT_Operator_BitwiseXor:
        return make_unique<ASTExprValue>(INT_CONST, to_string(i1 ^ i2), v1->line, v1->col);
      default:
        ASTNode::SyntaxError(binary, "Invalid operator in int expression");
    }
  }
  ASTNode::SyntaxError(binary, "Binary expressions are only supported for int, float and string types");
  return nullptr;
}