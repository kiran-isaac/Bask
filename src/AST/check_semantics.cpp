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
    ASTNode::ValueError(this->line, this->col, "Variable " + name + " already exists");
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
        this->line, this->col, "Binary operation can only be applied to primitive types");
  }

  type = KL_Type(lhs_type);

  if (lhs_type.primitive == KL_STRING || rhs_type.primitive == KL_STRING) {
    auto t1 = lhs_type.primitive == KL_STRING ? lhs_type : rhs_type;
    auto t2 = lhs_type.primitive == KL_STRING ? rhs_type : lhs_type;

    if (t2.primitive != KL_STRING && t2.primitive != KL_CHAR) {
      ASTNode::TypeError(this->line, this->col, "Invalid type in string expression");
    }

    // t1 is definitely a string
    // t2 could be a string or char
    switch (op) {
      // Valid operations for t2 being a string or char
      case KL_TT_Operator_Add:
        break;

      // Valid operations for t2 being a string
      case KL_TT_Operator_Equal:
      case KL_TT_Operator_NotEqual:
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col, "Invalid operator in " + t1.to_string() + " and " + t2.to_string() + " binary expression");
    }
  } else if (lhs_type.primitive == KL_FLOAT || rhs_type.primitive == KL_FLOAT) {
    auto t1 = lhs_type.primitive == KL_FLOAT ? lhs_type : rhs_type;
    auto t2 = lhs_type.primitive == KL_FLOAT ? rhs_type : lhs_type;

    if (t2.primitive != KL_FLOAT && t2.primitive != KL_INT) {
      ASTNode::TypeError(this->line, this->col, "Invalid type in float expression");
    }

    // t1 is definitely a float
    // t2 could be a float or an int
    switch (op) {
      // Valid operations for t2 being a float or an int
      case KL_TT_Operator_Add:
      case KL_TT_Operator_Sub:
      case KL_TT_Operator_Mul:
      case KL_TT_Operator_Div:
      case KL_TT_Operator_Mod:
        break;

      // Valid operations for t2 being a float
      case KL_TT_Operator_Equal:
      case KL_TT_Operator_NotEqual:
      case KL_TT_Operator_Less:
      case KL_TT_Operator_LessEqual:
      case KL_TT_Operator_Greater:
      case KL_TT_Operator_GreaterEqual:
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col, "Invalid operator in " + t1.to_string() + " and " + t2.to_string() + " binary expression");
    }
  } else if (lhs_type.primitive == KL_INT || rhs_type.primitive == KL_INT) {
    auto t1 = lhs_type.primitive == KL_INT ? lhs_type : rhs_type;
    auto t2 = lhs_type.primitive == KL_INT ? rhs_type : lhs_type;

    if (t2.primitive != KL_INT && t2.primitive != KL_CHAR) {
      ASTNode::TypeError(this->line, this->col, "Invalid type in int expression");
    }

    // t1 is definitely an int
    // t2 could be an int or a char
    switch (op) {
      // Valid operations for t2 being an int or a char
      // Arithmetic
      case KL_TT_Operator_Add:
      case KL_TT_Operator_Sub:
      case KL_TT_Operator_Mul:
      case KL_TT_Operator_Div:
      case KL_TT_Operator_Mod:
      case KL_TT_Operator_Shl:
      case KL_TT_Operator_Shr:
      // Comparisons 
      case KL_TT_Operator_Less:
      case KL_TT_Operator_LessEqual:
      case KL_TT_Operator_Greater:
      case KL_TT_Operator_GreaterEqual:
      case KL_TT_Operator_Equal:
      case KL_TT_Operator_NotEqual:
        break;

      // Valid operations for t2 being an int
      case KL_TT_Operator_BitwiseAnd:
      case KL_TT_Operator_BitwiseOr:
      case KL_TT_Operator_BitwiseXor:
        if (t2.primitive == KL_INT) break;

      default:
        ASTNode::SyntaxError(this->line, this->col, "Invalid operator in " + t1.to_string() + " and " + t2.to_string() + " binary expression");
    }
  } else if (lhs_type.primitive == KL_BOOL && rhs_type.primitive == KL_BOOL) {
    // t1 and t2 are definitely bool
    switch (op) {
      // Valid operations for bool
      case KL_TT_Operator_Equal:
      case KL_TT_Operator_NotEqual:
      case KL_TT_Operator_LogicalAnd:
      case KL_TT_Operator_LogicalOr:
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col, "Invalid operator in bool expression");
    }
  } else if (lhs_type.primitive == KL_CHAR && rhs_type.primitive == KL_CHAR) {
    // t1 and t2 are definitely char
    switch (op) {
      // Valid operations for char
      case KL_TT_Operator_Equal:
      case KL_TT_Operator_NotEqual:
      case KL_TT_Operator_Less:
      case KL_TT_Operator_LessEqual:
      case KL_TT_Operator_Greater:
      case KL_TT_Operator_GreaterEqual:
      case KL_TT_Operator_BitwiseAnd:
      case KL_TT_Operator_BitwiseOr:
      case KL_TT_Operator_BitwiseXor:
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col, "Invalid operator in " + lhs_type.to_string() + " and " + rhs_type.to_string() + " binary expression");
    }
  } else {
    ASTNode::SyntaxError(this->line, this->col, "Invalid type in binary expression");
  }
}

void ASTExprUnary::check_semantics() {
  // TODO type check operation
  expr->check_semantics();
  KL_Type expr_type = expr->get_expr_type();
  if (expr_type.kind != KL_PRIMITIVE) {
    ASTNode::SyntaxError(this->line, this->col, "Unary operation can only be applied to primitive types");
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
