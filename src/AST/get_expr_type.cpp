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
  if (checked) {
    return type;
  }

  check_semantics();
  return type;
}