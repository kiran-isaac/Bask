#include <gtest/gtest.h>

#include "../utils/utils.h"
#include "AST/AST.h"
#include "options.h"
#include "parser.h"
#include "types.h"

TEST(GetExprType, Unary) {
  auto file = insertIntoTempFile("int main() { -5; }");
  const char* argv[] = {"BASK", file};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto program = parser.parse();
  auto expr_stmt = program->get_function("main")->body->get_statement(0);
  auto expr = dynamic_cast<ASTStmtExpr*>(expr_stmt)->expr.get();

  ASSERT_EQ(expr->get_expr_type().primitive, BASK_INT_PRIMITIVE);
  ASSERT_EQ(expr->get_expr_type().kind, BASK_PRIMITIVE_TYPEKIND);
  ASSERT_EQ(expr->get_expr_type().is_const, true);
}

TEST(GetExprType, Binary) {
  // Based off types from spec/expressions.md:
  /*
  ## Binary expressions
  These are in order of matching precedence.

  ### Arithmetic: +, -, *, /, %
  FLOAT + FLOAT = FLOAT
  FLOAT + INT = FLOAT
  INT + INT = INT

  ## String concatenation: +
  STRING + STRING = STRING

  ## Equality: ==, !=
  FLOAT == FLOAT = BOOL
  INT == FLOAT = BOOL
  INT == INT = BOOL
  STRING == STRING = BOOL
  CHAR == CHAR = BOOL
  BOOL == BOOL = BOOL

  ## Comparison: <, <=, >, >=
  FLOAT < FLOAT = BOOL
  FLOAT < INT = BOOL
  INT < INT = BOOL

  ## Logical: &&, ||
  BOOL && BOOL = BOOL
  BOOL || BOOL = BOOL

  ## Bitwise: &, |, ^, <<, >>
  INT & INT = INT
  INT | INT = INT
  INT ^ INT = INT
  INT << INT = INT
  INT >> INT = INT 
  */
  struct ExpressionType {
    BASK_PrimitiveType lhs;
    BASK_PrimitiveType rhs;
    bool valid;
    BASK_PrimitiveType result;
  };
  
  // Put all the valid types in a map. Dont need to worry about the order of the types
  // because the test will check for both orders
  map<BASK_TokenType, vector<ExpressionType>> valid_ops = {
    {BASK_TT_Operator_Add, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE},
      {BASK_STRING_PRIMITIVE, BASK_STRING_PRIMITIVE, true, BASK_STRING_PRIMITIVE}
    }},
    {BASK_TT_Operator_Sub, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_Mul, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_Div, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_Mod, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_FLOAT_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_Equal, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_STRING_PRIMITIVE, BASK_STRING_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_CHAR_PRIMITIVE, BASK_CHAR_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_BOOL_PRIMITIVE, BASK_BOOL_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},

    {BASK_TT_Operator_NotEqual, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_STRING_PRIMITIVE, BASK_STRING_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_CHAR_PRIMITIVE, BASK_CHAR_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_BOOL_PRIMITIVE, BASK_BOOL_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},
    {BASK_TT_Operator_Less, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},
    {BASK_TT_Operator_LessEqual, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},
    {BASK_TT_Operator_Greater, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},
    {BASK_TT_Operator_GreaterEqual, {
      {BASK_FLOAT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_FLOAT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE},
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},

    {BASK_TT_Operator_LogicalAnd, {
      {BASK_BOOL_PRIMITIVE, BASK_BOOL_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},
    {BASK_TT_Operator_LogicalOr, {
      {BASK_BOOL_PRIMITIVE, BASK_BOOL_PRIMITIVE, true, BASK_BOOL_PRIMITIVE}
    }},

    {BASK_TT_Operator_BitwiseAnd, {
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_BitwiseOr, {
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_BitwiseXor, {
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_Shl, {
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }},
    {BASK_TT_Operator_Shr, {
      {BASK_INT_PRIMITIVE, BASK_INT_PRIMITIVE, true, BASK_INT_PRIMITIVE}
    }}
  };

  map<BASK_TokenType, vector<ExpressionType>> all_ops = {};

      vector<BASK_PrimitiveType> primitives = {
          BASK_INT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, BASK_BOOL_PRIMITIVE, BASK_CHAR_PRIMITIVE, BASK_STRING_PRIMITIVE,
      };


  // Add in all the invalid orders
  for (auto& [op, types] : valid_ops) {
    for (auto& p1 : primitives) {
      for (auto& p2 : primitives) {
        bool contained = false;
        BASK_PrimitiveType result = BASK_VOID_PRIMITIVE;
        for (auto& type : types) {
          if ((type.lhs == p1 && type.rhs == p2) || (type.rhs == p1 && type.lhs == p2)) {
            contained = true;
            result = type.result;
            break;
          }
        }
        all_ops[op].push_back({p1, p2, contained, result});
      }
    }
  }

   int i = 0;
   int j = 0;
  for (auto& [op, types] : all_ops) {
    ASSERT_EQ(types.size(), primitives.size() * primitives.size());
    for (auto& type : types) {
      unique_ptr<ASTExprBinary> expr = make_unique<ASTExprBinary>(
          make_unique<ASTExprConstantValue>(BASK_Type(true, type.lhs), "", 0, 0),
          make_unique<ASTExprConstantValue>(BASK_Type(true, type.rhs), "", 0, 0), op,
          0, 0);

      if (type.valid) {
        try{
          auto expr_type = expr->get_expr_type();
          ASSERT_EQ(expr_type.primitive, type.result);
          ASSERT_EQ(expr_type.kind, BASK_PRIMITIVE_TYPEKIND);
          ASSERT_EQ(expr_type.is_const, true);
        } catch (std::runtime_error e) {
          FAIL() << "Expected no runtime_error";
        }
      } else {
        try{
          auto expr_type = expr->get_expr_type();
          FAIL() << "Expected runtime_error";
        } catch (std::runtime_error e) {
        }
      }
      j++;
    }
    i++;
  }
}