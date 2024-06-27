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
}

TEST(GetExprType, Binary) {
  auto file = insertIntoTempFile("int main() { 5 + 5; }");
  const char* argv[] = {"KL", file};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto program = parser.parse();
  auto expr_stmt = program->get_function("main")->body->get_statement(0);
  auto expr = dynamic_cast<ASTStmtExpr*>(expr_stmt)->expr.get();

  printf("%s\n", expr->get_expr_type().to_string().c_str());

  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT);
}