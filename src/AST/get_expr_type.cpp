#include "AST/AST.h"
#include "symtab.h"

KL_Type ASTExprIdentifier::get_expr_type() {
  auto type = SYMTAB.get_name_type(name);
  if (!type) {
    ASTNode::ValueError(this->line, this->col, "Variable " + name + " does not exist");
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