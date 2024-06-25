#include "AST/AST.h"
#include "symtab.h"

KL_Type ASTExprIdentifier::get_expr_type() {
  auto type = SYMTAB.get_name_type(name);
  if (!type) {
    throw std::runtime_error("Variable " + name + " not found");
  }
  return *type;
}

KL_Type ASTExprBinary::get_expr_type() {
  KL_Type lhs_type = lhs->get_expr_type();
  KL_Type rhs_type = rhs->get_expr_type();
  if (lhs_type != rhs_type) {
    throw std::runtime_error("Type mismatch in binary expression");
  }
}