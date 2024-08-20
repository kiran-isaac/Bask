#include "AST/AST.h"
#include "AST/AST_Expressions.h"
#include "tokens.h"

void ASTExprIdentifier::check_semantics() {
  // TODO check if variable exists
}

void ASTStmtAssignment::check_semantics() {
  auto name = identifier->name;
  // check exists
  auto exists = SYMTAB.get_name_type(name);
  if (exists == nullopt) {
    ASTNode::ValueError(this->line, this->col,
                        "Variable " + name + " does not exist");
  }

  BASK_Type lhstype = exists.value();
  BASK_Type rhstype = value->get_expr_type();
  if (lhstype.kind != rhstype.kind) {
    ASTNode::TypeError(this->line, this->col, "Type mismatch in declaration");
  }
  if (lhstype.is_const && !rhstype.is_const) {
    ASTNode::TypeError(this->line, this->col,
                       "Cannot assign non-const to const");
  }

  switch (lhstype.kind) {
    case BASK_PRIMITIVE_TYPEKIND:
      if (lhstype.primitive != rhstype.primitive) {
        ASTNode::TypeError(this->line, this->col,
                           "Type mismatch in declaration");
      }
      break;
    case BASK_ARRAY_TYPEKIND:
      if (lhstype.array_sizes != rhstype.array_sizes) {
        ASTNode::TypeError(this->line, this->col,
                           "Incompatible array sizes in declaration");
      }
      if (lhstype.primitive != rhstype.primitive) {
        ASTNode::TypeError(this->line, this->col,
                           "Incompatible array type in declaration");
      }
      break;
    case BASK_FUNCTION_TYPEKIND:
      throw std::runtime_error(
          "UNREACHABLE: Function type in declaration not supported");
    default:
      ASTNode::TypeError(this->line, this->col, "Type mismatch in declaration");
  }
}

void ASTStmtDecl::check_semantics() {
  auto exists = SYMTAB.get_name_type(identifier->name);
  if (exists) {
    ASTNode::ValueError(this->line, this->col,
                        "Variable " + identifier->name + " already exists");
  }
  BASK_Type lhstype = BASK_Type(type->type);
  BASK_Type rhstype = value->get_expr_type();
  if (lhstype.kind != rhstype.kind) {
    ASTNode::TypeError(this->line, this->col, "Type mismatch in declaration. Expected '" +
        lhstype.to_string() + "' but got '" + rhstype.to_string() + "'");
  }
  if (lhstype.is_const && !rhstype.is_const) {
    ASTNode::TypeError(this->line, this->col,
                       "Cannot assign non-const to const");
  }

  switch (lhstype.kind) {
    case BASK_PRIMITIVE_TYPEKIND:
      if (lhstype.primitive != rhstype.primitive) {
        ASTNode::TypeError(this->line, this->col,
                           "Type mismatch in declaration. Expected '" +
                               lhstype.to_string() + "' but got '" +
                               rhstype.to_string() + "'");
      }
      break;
    case BASK_ARRAY_TYPEKIND:
      if (lhstype.array_sizes != rhstype.array_sizes) {
        ASTNode::TypeError(this->line, this->col,
                           "Incompatible array sizes in declaration. Expected '" +
                               lhstype.to_string() + "' but got '" +
                               rhstype.to_string() + "'");
      }
      if (lhstype.primitive != rhstype.primitive) {
        ASTNode::TypeError(this->line, this->col,
                           "Incompatible array type in declaration. Expected '" +
                               lhstype.to_string() + "' but got '" +
                               rhstype.to_string() + "'");
      }
      
      break;
    case BASK_FUNCTION_TYPEKIND:
      throw std::runtime_error(
          "UNREACHABLE: Function type in declaration not supported");
    default:
      ASTNode::TypeError(this->line, this->col, "Type mismatch in declaration");
  }

  SYMTAB.add_name(identifier->name, lhstype);
}

void ASTExprBinary::check_semantics() {
  // TODO type check operation
  lhs->check_semantics();
  rhs->check_semantics();

  BASK_Type lhs_type = lhs->get_expr_type();
  BASK_Type rhs_type = rhs->get_expr_type();

  if (lhs_type.kind != BASK_PRIMITIVE_TYPEKIND || rhs_type.kind != BASK_PRIMITIVE_TYPEKIND) {
    ASTNode::SyntaxError(
        this->line, this->col,
        "Binary operation can only be applied to primitive types");
  }

  bool is_const = lhs_type.is_const && rhs_type.is_const;

  if (lhs_type.primitive == BASK_STRING_PRIMITIVE || rhs_type.primitive == BASK_STRING_PRIMITIVE) {
    auto t1 = lhs_type.primitive == BASK_STRING_PRIMITIVE ? lhs_type : rhs_type;
    auto t2 = lhs_type.primitive == BASK_STRING_PRIMITIVE ? rhs_type : lhs_type;

    if (t2.primitive != BASK_STRING_PRIMITIVE) {
      ASTNode::TypeError(this->line, this->col,
                         "Invalid type in string expression");
    }

    // both t1 and t2 are strings
    switch (op) {
      // Valid operations for t2 being a string or char
      case BASK_TT_Operator_Add:
        type = BASK_Type(is_const, BASK_STRING_PRIMITIVE);
        break;

      // Valid operations for t2 being a string
      case BASK_TT_Operator_Equal:
      case BASK_TT_Operator_NotEqual:
        type = BASK_Type(is_const, BASK_BOOL_PRIMITIVE);
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col,
                             "Invalid operator in " + t1.to_string() + " and " +
                                 t2.to_string() + " binary expression");
    }
  } else if (lhs_type.primitive == BASK_FLOAT_PRIMITIVE || rhs_type.primitive == BASK_FLOAT_PRIMITIVE) {
    auto t1 = lhs_type.primitive == BASK_FLOAT_PRIMITIVE ? lhs_type : rhs_type;
    auto t2 = lhs_type.primitive == BASK_FLOAT_PRIMITIVE ? rhs_type : lhs_type;

    if (t2.primitive != BASK_FLOAT_PRIMITIVE && t2.primitive != BASK_INT_PRIMITIVE) {
      ASTNode::TypeError(this->line, this->col,
                         "Invalid type in float expression");
    }

    // t1 is definitely a float
    // t2 could be a float or an int
    switch (op) {
      // Valid operations for t2 being a float or an int
      case BASK_TT_Operator_Add:
      case BASK_TT_Operator_Sub:
      case BASK_TT_Operator_Mul:
      case BASK_TT_Operator_Div:
      case BASK_TT_Operator_Mod:
        type = BASK_Type(is_const, BASK_FLOAT_PRIMITIVE);
        break;

      // Valid operations for t2 being a float
      case BASK_TT_Operator_Equal:
      case BASK_TT_Operator_NotEqual:
      case BASK_TT_Operator_Less:
      case BASK_TT_Operator_LessEqual:
      case BASK_TT_Operator_Greater:
      case BASK_TT_Operator_GreaterEqual:
        type = BASK_Type(is_const, BASK_BOOL_PRIMITIVE);
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col,
                             "Invalid operator in " + t1.to_string() + " and " +
                                 t2.to_string() + " binary expression");
    }
  } else if (lhs_type.primitive == BASK_INT_PRIMITIVE || rhs_type.primitive == BASK_INT_PRIMITIVE) {
    auto t1 = lhs_type.primitive == BASK_INT_PRIMITIVE ? lhs_type : rhs_type;
    auto t2 = lhs_type.primitive == BASK_INT_PRIMITIVE ? rhs_type : lhs_type;

    if (t2.primitive != BASK_INT_PRIMITIVE) {
      ASTNode::TypeError(this->line, this->col,
                         "Invalid type in int expression");
    }

    // t1 is definitely an int
    // t2 could be an int or a char
    switch (op) {
      // Valid operations for t2 being an int or a char
      // Arithmetic
      case BASK_TT_Operator_Add:
      case BASK_TT_Operator_Sub:
      case BASK_TT_Operator_Mul:
      case BASK_TT_Operator_Div:
      case BASK_TT_Operator_Mod:
      case BASK_TT_Operator_Shl:
      case BASK_TT_Operator_Shr:
        type = BASK_Type(is_const, BASK_INT_PRIMITIVE);
        break;
      // Comparisons
      case BASK_TT_Operator_Less:
      case BASK_TT_Operator_LessEqual:
      case BASK_TT_Operator_Greater:
      case BASK_TT_Operator_GreaterEqual:
      case BASK_TT_Operator_Equal:
      case BASK_TT_Operator_NotEqual:
        type = BASK_Type(is_const, BASK_BOOL_PRIMITIVE);
        break;

      // Valid operations for t2 being an int
      case BASK_TT_Operator_BitwiseAnd:
      case BASK_TT_Operator_BitwiseOr:
      case BASK_TT_Operator_BitwiseXor:
        if (t2.primitive == BASK_INT_PRIMITIVE) {
          type = BASK_Type(is_const, BASK_INT_PRIMITIVE);
          break;
        }

      default:
        ASTNode::SyntaxError(this->line, this->col,
                             "Invalid operator in " + t1.to_string() + " and " +
                                 t2.to_string() + " binary expression");
    }
  } else if (lhs_type.primitive == BASK_BOOL_PRIMITIVE && rhs_type.primitive == BASK_BOOL_PRIMITIVE) {
    // t1 and t2 are definitely bool
    switch (op) {
      // Valid operations for bool
      case BASK_TT_Operator_Equal:
      case BASK_TT_Operator_NotEqual:
      case BASK_TT_Operator_LogicalAnd:
      case BASK_TT_Operator_LogicalOr:
        type = BASK_Type(is_const, BASK_BOOL_PRIMITIVE);
        break;

      default:
        ASTNode::SyntaxError(this->line, this->col,
                             "Invalid operator in bool expression");
    }
  } else if (lhs_type.primitive == BASK_CHAR_PRIMITIVE && rhs_type.primitive == BASK_CHAR_PRIMITIVE) {
    // t1 and t2 are definitely char
    switch (op) {
      // Valid operations for char
      case BASK_TT_Operator_Equal:
      case BASK_TT_Operator_NotEqual:
        type = BASK_Type(is_const, BASK_BOOL_PRIMITIVE);
        break;
      default:
        ASTNode::SyntaxError(this->line, this->col,
                             "Invalid operator in " + lhs_type.to_string() +
                                 " and " + rhs_type.to_string() +
                                 " binary expression");
    }
  } else {
    ASTNode::SyntaxError(this->line, this->col,
                         "Invalid type in binary expression");
  }
}

void ASTExprUnary::check_semantics() {
  // TODO type check operation
  expr->check_semantics();
  BASK_Type expr_type = expr->get_expr_type();
  if (expr_type.kind != BASK_PRIMITIVE_TYPEKIND) {
    ASTNode::SyntaxError(
        this->line, this->col,
        "Unary operation can only be applied to primitive types");
  }
  switch (op) {
    case BASK_TT_Operator_Sub:
      if (expr_type.primitive != BASK_INT_PRIMITIVE && expr_type.primitive != BASK_FLOAT_PRIMITIVE) {
        throw std::runtime_error(
            "Unary minus can only be applied to int or float");
      }
      break;
    case BASK_TT_Operator_LogicalNot:
      if (expr_type.primitive != BASK_BOOL_PRIMITIVE) {
        throw std::runtime_error(
            "Unary logical not can only be applied to bool");
      }
      break;
    case BASK_TT_Operator_BitwiseNot:
      if (expr_type.primitive != BASK_INT_PRIMITIVE) {
        throw std::runtime_error("Unary binary can only be applied to int");
      }
    default:
      throw std::runtime_error("Unknown unary operator");
  }
}

BASK_Type ASTExprUnary::get_expr_type() { return BASK_Type(expr->get_expr_type()); }
