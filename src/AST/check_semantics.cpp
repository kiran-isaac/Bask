#include "AST/AST.h"
#include "AST/AST_Expressions.h"

void ASTExprIdentifier::check_semantics() {
  // TODO check if variable exists
}

void ASTStmtAssignment::check_semantics() {
  // TODO check if variable exists and assignment is valid
  value->check_semantics();
}

void ASTStmtDecl::check_semantics() {
  auto exists = SYMTAB.get_name_type(name);
  if (exists) {
    ASTNode::ValueError(this, "Variable " + name + " already exists");
  }
  SYMTAB.add_name(name, type->type);
  if (value) {
    value->check_semantics();
  }
}

void ASTExprBinary::check_semantics() {
  // TODO type check operation
  lhs->check_semantics();
  rhs->check_semantics();
}

void ASTExprUnary::check_semantics() {
  // TODO type check operation
  expr->check_semantics();
  KL_Type expr_type = expr->get_expr_type();
  if (expr_type.kind != KL_PRIMITIVE) {
    throw std::runtime_error(
        "Unary minus can only be applied to primitive types");
  }
  switch (op) {
  case KL_TT_Operator_Sub:
    if (expr_type.primitive != KL_INT && expr_type.primitive != KL_FLOAT) {
      throw std::runtime_error(
          "Unary minus can only be applied to int or float");
    }
    break;
  case KL_TT_Operator_LogicalNot:
    if (expr_type.primitive != KL_BOOL) {
      throw std::runtime_error("Unary logical not can only be applied to bool");
    }
    break;
    case KL_TT_Operator_BitwiseNot:
    if (expr_type.primitive != KL_INT) {
      throw std::runtime_error("Unary binary can only be applied to int");
    }
  default:
    throw std::runtime_error("Unknown unary operator");
  }
}

KL_Type ASTExprUnary::get_expr_type() { return KL_Type(expr->get_expr_type()); }
