#include "AST/AST.h"
#include "symtab.h"

BASK_Type ASTExprIdentifier::get_expr_type() {
  auto type = SYMTAB.get_name_type(name);
  if (!type) {
    ASTNode::ValueError(this->line, this->col, "Variable " + name + " does not exist");
  }
  return *type;
}

BASK_Type ASTExprBinary::get_expr_type() {
  if (checked) {
    return type;
  }

  check_semantics();
  return type;
}