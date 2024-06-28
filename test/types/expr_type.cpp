#include <gtest/gtest.h>

#include "../utils/utils.h"
#include "AST/AST.h"
#include "options.h"
#include "parser.h"
#include "types.h"

TEST(GetExprType, Unary) {
  auto file = insertIntoTempFile("int main() { -5; }");
  const char* argv[] = {"KL", file};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto program = parser.parse();
  auto expr_stmt = program->get_function("main")->body->get_statement(0);
  auto expr = dynamic_cast<ASTStmtExpr*>(expr_stmt)->expr.get();

  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT);
  ASSERT_EQ(expr->get_expr_type().kind, KL_PRIMITIVE);
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
    KL_PrimitiveType lhs;
    KL_PrimitiveType rhs;
    bool valid;
    KL_PrimitiveType result;
  };
  
  // Put all the valid types in a map. Dont need to worry about the order of the types
  // because the test will check for both orders
  map<KL_TokenType, vector<ExpressionType>> valid_ops = {
    {KL_TT_Operator_Add, {
      {KL_FLOAT, KL_FLOAT, true, KL_FLOAT},
      {KL_FLOAT, KL_INT, true, KL_FLOAT},
      {KL_INT, KL_INT, true, KL_INT},
      {KL_STRING, KL_STRING, true, KL_STRING}
    }},
    {KL_TT_Operator_Sub, {
      {KL_FLOAT, KL_FLOAT, true, KL_FLOAT},
      {KL_FLOAT, KL_INT, true, KL_FLOAT},
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_Mul, {
      {KL_FLOAT, KL_FLOAT, true, KL_FLOAT},
      {KL_FLOAT, KL_INT, true, KL_FLOAT},
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_Div, {
      {KL_FLOAT, KL_FLOAT, true, KL_FLOAT},
      {KL_FLOAT, KL_INT, true, KL_FLOAT},
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_Mod, {
      {KL_FLOAT, KL_FLOAT, true, KL_FLOAT},
      {KL_FLOAT, KL_INT, true, KL_FLOAT},
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_Equal, {
      {KL_FLOAT, KL_FLOAT, true, KL_BOOL},
      {KL_FLOAT, KL_INT, true, KL_BOOL},
      {KL_INT, KL_INT, true, KL_BOOL},
      {KL_STRING, KL_STRING, true, KL_BOOL},
      {KL_CHAR, KL_CHAR, true, KL_BOOL},
      {KL_BOOL, KL_BOOL, true, KL_BOOL}
    }},

    {KL_TT_Operator_NotEqual, {
      {KL_FLOAT, KL_FLOAT, true, KL_BOOL},
      {KL_FLOAT, KL_INT, true, KL_BOOL},
      {KL_INT, KL_INT, true, KL_BOOL},
      {KL_STRING, KL_STRING, true, KL_BOOL},
      {KL_CHAR, KL_CHAR, true, KL_BOOL},
      {KL_BOOL, KL_BOOL, true, KL_BOOL}
    }},
    {KL_TT_Operator_Less, {
      {KL_FLOAT, KL_FLOAT, true, KL_BOOL},
      {KL_FLOAT, KL_INT, true, KL_BOOL},
      {KL_INT, KL_INT, true, KL_BOOL}
    }},
    {KL_TT_Operator_LessEqual, {
      {KL_FLOAT, KL_FLOAT, true, KL_BOOL},
      {KL_FLOAT, KL_INT, true, KL_BOOL},
      {KL_INT, KL_INT, true, KL_BOOL}
    }},
    {KL_TT_Operator_Greater, {
      {KL_FLOAT, KL_FLOAT, true, KL_BOOL},
      {KL_FLOAT, KL_INT, true, KL_BOOL},
      {KL_INT, KL_INT, true, KL_BOOL}
    }},
    {KL_TT_Operator_GreaterEqual, {
      {KL_FLOAT, KL_FLOAT, true, KL_BOOL},
      {KL_FLOAT, KL_INT, true, KL_BOOL},
      {KL_INT, KL_INT, true, KL_BOOL}
    }},

    {KL_TT_Operator_LogicalAnd, {
      {KL_BOOL, KL_BOOL, true, KL_BOOL}
    }},
    {KL_TT_Operator_LogicalOr, {
      {KL_BOOL, KL_BOOL, true, KL_BOOL}
    }},

    {KL_TT_Operator_BitwiseAnd, {
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_BitwiseOr, {
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_BitwiseXor, {
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_Shl, {
      {KL_INT, KL_INT, true, KL_INT}
    }},
    {KL_TT_Operator_Shr, {
      {KL_INT, KL_INT, true, KL_INT}
    }}
  };

  map<KL_TokenType, vector<ExpressionType>> all_ops = {};

      vector<KL_PrimitiveType> primitives = {
          KL_INT, KL_FLOAT, KL_BOOL, KL_CHAR, KL_STRING,
      };


  // Add in all the invalid orders
  for (auto& [op, types] : valid_ops) {
    for (auto& p1 : primitives) {
      for (auto& p2 : primitives) {
        bool contained = false;
        KL_PrimitiveType result = KL_VOID;
        for (auto& type : types) {
          if (type.lhs == p1 && type.rhs == p2 || type.rhs == p1 && type.lhs == p2) {
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
          make_unique<ASTExprConstantValue>(KL_Type(true, type.lhs), "", 0, 0),
          make_unique<ASTExprConstantValue>(KL_Type(true, type.rhs), "", 0, 0), op,
          0, 0);

      if (type.valid) {
        try{
          auto expr_type = expr->get_expr_type();
          ASSERT_EQ(expr_type.primitive, type.result);
          ASSERT_EQ(expr_type.kind, KL_PRIMITIVE);
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