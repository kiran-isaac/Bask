#include "AST/AST.h"

void ASTExprIdentifier::check_semantics() {
  // TODO check if variable exists
}

void ASTStmtAssignment::check_semantics() {
  // TODO check if variable exists and assignment is valid
  value->check_semantics();
}

void ASTStmtDecl::check_semantics() {
  // TODO check if variable exists and assignment is valid
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
}

