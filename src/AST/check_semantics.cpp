#include "AST/AST.h"
#include "AST/AST_Expressions.h"
#include "tokens.h"

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

  KL_Type lhs_type = lhs->get_expr_type();
  KL_Type rhs_type = rhs->get_expr_type();

  if (lhs_type.kind != KL_PRIMITIVE || rhs_type.kind != KL_PRIMITIVE) {
    ASTNode::SyntaxError(
        this, "Binary operation can only be applied to primitive types");
  }

  // Get the type of the result of the operation and check if it is 
  switch (lhs_type.primitive) {
    case KL_INT:
      switch (rhs_type.primitive) {
        case KL_INT:
          type = INT_CONST;
          break;
        case KL_FLOAT:
          type = FLOAT_CONST;
          break;
        default:
          ASTNode::TypeError(this, "Cannot use operator " +
                                       std::string(token_type_to_string(op)) +
                                       " with types " + lhs_type.to_string() +
                                       " and " + rhs_type.to_string());
      }
      break;
    case KL_FLOAT:
      switch (rhs_type.primitive) {
        case KL_INT:
        case KL_FLOAT:
          type = FLOAT_CONST;
          break;
        default:
          ASTNode::TypeError(this, "Cannot use operator " +
                                       std::string(token_type_to_string(op)) +
                                       " with types " + lhs_type.to_string() +
                                       " and " + rhs_type.to_string());
      }
      break;
    case KL_BOOL:
      switch (rhs_type.primitive) {
        case KL_BOOL:
          type = BOOL_CONST;
          break;
        default:
          ASTNode::TypeError(this, "Cannot use operator " +
                                       std::string(token_type_to_string(op)) +
                                       " with types " + lhs_type.to_string() +
                                       " and " + rhs_type.to_string());
      }
      break;
    case KL_CHAR:
      switch (rhs_type.primitive) {
        case KL_CHAR:
          type = CHAR_CONST;
          break;
        case KL_STRING:
          type = STRING_CONST;
          break;
        case KL_INT:
          type = INT_CONST;
        default:
          ASTNode::TypeError(this, "Cannot use operator " +
                                       std::string(token_type_to_string(op)) +
                                       " with types " + lhs_type.to_string() +
                                       " and " + rhs_type.to_string());
      }
      break;
    case KL_STRING:
      switch (rhs_type.primitive) {
        case KL_CHAR:
        case KL_STRING:
          type = STRING_CONST;
          break;
        default:
          ASTNode::TypeError(this, "Cannot use operator " +
                                       std::string(token_type_to_string(op)) +
                                       " with types " + lhs_type.to_string() +
                                       " and " + rhs_type.to_string());
      }
      break;
  }

  // type is now set to the type of the result of the operation. Check if the
  // operation can produce a result of that type
  switch (op) {
    // Add: valid for string (concatenation), char, int, float (addition)
    case KL_TT_Operator_Add:
      if (type != STRING_CONST && type != FLOAT_CONST && type != INT_CONST ) {
        ASTNode::TypeError(this, "Cannot use operator " +
                                     std::string(token_type_to_string(op)) +
                                     " with types " + lhs_type.to_string() +
                                     " and " + rhs_type.to_string());
      }
      break;

    // Sub, Mul, Div and Mod: valid for int and float
    case KL_TT_Operator_Sub:
    case KL_TT_Operator_Mul:
    case KL_TT_Operator_Div:
    case KL_TT_Operator_Mod:
      if (type != FLOAT_CONST && type != INT_CONST) {
        ASTNode::TypeError(this, "Cannot use operator " +
                                     std::string(token_type_to_string(op)) +
                                     " with types " + lhs_type.to_string() +
                                     " and " + rhs_type.to_string());
      }
      break;

    // Bitwise operators: valid for int and char
    case KL_TT_Operator_BitwiseAnd:
    case KL_TT_Operator_BitwiseOr:
    case KL_TT_Operator_BitwiseXor:
    case KL_TT_Operator_BitwiseNot:
      if (type != INT_CONST && type != CHAR_CONST) {
        ASTNode::TypeError(this, "Cannot use operator " +
                                     std::string(token_type_to_string(op)) +
                                     " with types " + lhs_type.to_string() +
                                     " and " + rhs_type.to_string());
      }
      break;

    // Logical operators: valid for bool
    case KL_TT_Operator_LogicalAnd:
    case KL_TT_Operator_LogicalOr:
    case KL_TT_Operator_LogicalNot:
      if (type != BOOL_CONST) {
        ASTNode::TypeError(this, "Cannot use operator " +
                                     std::string(token_type_to_string(op)) +
                                     " with types " + lhs_type.to_string() +
                                     " and " + rhs_type.to_string());
      }
      break;

    // Comparisons: valid for int, float and char
    case KL_TT_Operator_Less:
    case KL_TT_Operator_LessEqual:
    case KL_TT_Operator_Greater:
    case KL_TT_Operator_GreaterEqual:
      if (type != INT_CONST && type != FLOAT_CONST && type != CHAR_CONST) {
        ASTNode::TypeError(this, "Cannot use operator " +
                                     std::string(token_type_to_string(op)) +
                                     " with types " + lhs_type.to_string() +
                                     " and " + rhs_type.to_string());
      }
      break;

    // Shift operators: valid for int and char
    case KL_TT_Operator_Shl:
    case KL_TT_Operator_Shr:
      if (type != INT_CONST && type != CHAR_CONST) {
        ASTNode::TypeError(this, "Cannot use operator " +
                                     std::string(token_type_to_string(op)) +
                                     " with types " + lhs_type.to_string() +
                                     " and " + rhs_type.to_string());
      }
      break;

    // Valid for all types
    case KL_TT_Operator_Equal:
    case KL_TT_Operator_NotEqual:
      break;
  }
}

void ASTExprUnary::check_semantics() {
  // TODO type check operation
  expr->check_semantics();
  KL_Type expr_type = expr->get_expr_type();
  if (expr_type.kind != KL_PRIMITIVE) {
    ASTNode::SyntaxError(
        this, "Unary operation can only be applied to primitive types");
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
        throw std::runtime_error(
            "Unary logical not can only be applied to bool");
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
