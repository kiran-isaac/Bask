//
// Created by kiran on 5/21/24.
//

#include <AST.h>
#include <cmath>

unique_ptr<ASTExpr> ASTExpr::foldBinary(ASTExpr *expr) {
  auto binary = dynamic_cast<ASTExprBinary *>(expr);
  binary->lhs = fold(binary->lhs.get());
  binary->rhs = fold(binary->rhs.get());
  
  // Optimisation: Extract as many constants as possible. At the moment expressions aren't re-arranged if the operator is commutative
  
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