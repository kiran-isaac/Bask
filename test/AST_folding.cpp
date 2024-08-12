//
// Created by kiran on 5/21/24.
//

#include <gtest/gtest.h>

#include <memory>
#include <sstream>

#include "AST/AST.h"
#include "parser.h"
#include "utils/utils.h"

using namespace std;

// Binary expressions with both int types
TEST(ASTFolding, IntBinary) {
  string program =
      R"(int main() {
        int a = 5 + 5;
        int b = 5 - 5;
        int c = 5 * 5;
        int d = 5 / 5;
        int e = 5 % 5;
        int f = 5 < 5;
        int g = 5 <= 5;
        int h = 5 > 5;
        int i = 5 >= 5;
        int j = 5 == 5;
        int k = 5 != 5;
        int l = 5 & 5;
        int m = 5 | 5;
        int n = 5 ^ 5;
    })";

  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // 5 + 5
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "10");

  // 5 - 5
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0");

  // 5 * 5
  auto stmt3 = ast->get_function("main")->body->get_statement(2);
  decl = dynamic_cast<ASTStmtDecl *>(stmt3);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "25");

  // 5 / 5
  auto stmt4 = ast->get_function("main")->body->get_statement(3);
  decl = dynamic_cast<ASTStmtDecl *>(stmt4);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "1");

  // 5 % 5
  auto stmt5 = ast->get_function("main")->body->get_statement(4);
  decl = dynamic_cast<ASTStmtDecl *>(stmt5);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0");

  // 5 < 5
  auto stmt6 = ast->get_function("main")->body->get_statement(5);
  decl = dynamic_cast<ASTStmtDecl *>(stmt6);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5 <= 5
  auto stmt7 = ast->get_function("main")->body->get_statement(6);
  decl = dynamic_cast<ASTStmtDecl *>(stmt7);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5 > 5
  auto stmt8 = ast->get_function("main")->body->get_statement(7);
  decl = dynamic_cast<ASTStmtDecl *>(stmt8);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5 >= 5
  auto stmt9 = ast->get_function("main")->body->get_statement(8);
  decl = dynamic_cast<ASTStmtDecl *>(stmt9);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5 == 5
  auto stmt10 = ast->get_function("main")->body->get_statement(9);
  decl = dynamic_cast<ASTStmtDecl *>(stmt10);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);

  // 5 != 5
  auto stmt11 = ast->get_function("main")->body->get_statement(10);
  decl = dynamic_cast<ASTStmtDecl *>(stmt11);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5 & 5
  auto stmt12 = ast->get_function("main")->body->get_statement(11);
  decl = dynamic_cast<ASTStmtDecl *>(stmt12);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "5");

  // 5 | 5
  auto stmt13 = ast->get_function("main")->body->get_statement(12);
  decl = dynamic_cast<ASTStmtDecl *>(stmt13);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "5");

  // 5 ^ 5
  auto stmt14 = ast->get_function("main")->body->get_statement(13);
  decl = dynamic_cast<ASTStmtDecl *>(stmt14);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0");
}

// Binary expressions with both float types
TEST(ASTFolding, FloatBinary) {
  string program =
      R"(int main() {
        float a = 5.0 + 5.0;
        float b = 5.0 - 5.0;
        float c = 5.0 * 5.0;
        float d = 5.0 / 5.0;
        float e = 5.0 % 5.0;
        float f = 5.0 < 5.0;
        float g = 5.0 <= 5.0;
        float h = 5.0 > 5.0;
        float i = 5.0 >= 5.0;
        float j = 5.0 == 5.0;
        float k = 5.0 != 5.0;
    })";

  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // 5.0 + 5.0
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "10.000000");

  // 5.0 - 5.0
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0.000000");

  // 5.0 * 5.0
  auto stmt3 = ast->get_function("main")->body->get_statement(2);
  decl = dynamic_cast<ASTStmtDecl *>(stmt3);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "25.000000");

  // 5.0 / 5.0
  auto stmt4 = ast->get_function("main")->body->get_statement(3);
  decl = dynamic_cast<ASTStmtDecl *>(stmt4);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "1.000000");

  // 5.0 % 5.0
  auto stmt5 = ast->get_function("main")->body->get_statement(4);
  decl = dynamic_cast<ASTStmtDecl *>(stmt5);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0.000000");

  // 5.0 < 5.0
  auto stmt6 = ast->get_function("main")->body->get_statement(5);
  decl = dynamic_cast<ASTStmtDecl *>(stmt6);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5.0 <= 5.0
  auto stmt7 = ast->get_function("main")->body->get_statement(6);
  decl = dynamic_cast<ASTStmtDecl *>(stmt7);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5.0 > 5.0
  auto stmt8 = ast->get_function("main")->body->get_statement(7);
  decl = dynamic_cast<ASTStmtDecl *>(stmt8);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5.0 >= 5.0
  auto stmt9 = ast->get_function("main")->body->get_statement(8);
  decl = dynamic_cast<ASTStmtDecl *>(stmt9);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5.0 == 5.0
  auto stmt10 = ast->get_function("main")->body->get_statement(9);
  decl = dynamic_cast<ASTStmtDecl *>(stmt10);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5.0 != 5.0
  auto stmt11 = ast->get_function("main")->body->get_statement(10);
  decl = dynamic_cast<ASTStmtDecl *>(stmt11);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");
}

// Binary expressions with both string types
TEST(ASTFolding, StringBinary) {
  string program =
      R"(int main() {
        string d = "Hello, " + "World!";
        string a = "Hello, " + "World!";
        bool b = "Hello, " == "World!";
        bool c = "Hello, " != "World!";
    })";
  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // "Hello, " + "World!" + '\n'
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_STRING_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "Hello, World!");

  // "Hello, " + "World!"
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_STRING_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "Hello, World!");

  // "Hello, " == "World!"
  auto stmt3 = ast->get_function("main")->body->get_statement(2);
  decl = dynamic_cast<ASTStmtDecl *>(stmt3);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // "Hello, " != "World!"
  auto stmt4 = ast->get_function("main")->body->get_statement(3);
  decl = dynamic_cast<ASTStmtDecl *>(stmt4);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");
}

// Binary expressions with both bool types
TEST(ASTFolding, BoolBinary) {
  string program =
      R"(int main() {
        bool a = true && true;
        bool b = true || false;
        bool c = true && false;
        bool d = true || true;
        bool e = false && false;
        bool f = false || true;
        bool g = false && true;
        bool h = false || false;
    })";
  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // true && true
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // true || false
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // true && false
  auto stmt3 = ast->get_function("main")->body->get_statement(2);
  decl = dynamic_cast<ASTStmtDecl *>(stmt3);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // true || true
  auto stmt4 = ast->get_function("main")->body->get_statement(3);
  decl = dynamic_cast<ASTStmtDecl *>(stmt4);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // false && false
  auto stmt5 = ast->get_function("main")->body->get_statement(4);
  decl = dynamic_cast<ASTStmtDecl *>(stmt5);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // false || true
  auto stmt6 = ast->get_function("main")->body->get_statement(5);
  decl = dynamic_cast<ASTStmtDecl *>(stmt6);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // false && true
  auto stmt7 = ast->get_function("main")->body->get_statement(6);
  decl = dynamic_cast<ASTStmtDecl *>(stmt7);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // false || false
  auto stmt8 = ast->get_function("main")->body->get_statement(7);
  decl = dynamic_cast<ASTStmtDecl *>(stmt8);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");
}

// Binary expressions with both char types
TEST(ASTFolding, CharBinary) {
  string program =
      R"(int main() {
        char j = 'a' == 'b';
        char k = 'a' != 'b';
    })";
  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // 'a' == 'b'
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 'a' != 'b'
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");
}

TEST(ASTFolding, IntUnary) {
  string program =
      R"(int main() {
        int a = -5;
        int b = ~5;
    })";
  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // -5
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "-5");

  // ~5
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_INT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "-6");
}

TEST(ASTFolding, FloatUnary) {
  string program =
      R"(int main() {
        float a = -5.0;
    })";
  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // -5.0
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "-5.000000");
}

TEST(ASTFolding, BoolUnary) {
  string program =
      R"(int main() {
        bool a = !true;
    })";
  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // !true
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");
}

TEST(ASTFolding, IntFloatBinary) {
  string program =
      R"(int main() {
        float a = 5 + 5.0;
        float b = 5.0 - 5;
        float c = 5 * 5.0;
        float d = 5.0 / 5;
        float e = 5 % 5.0;
        float f = 5.0 < 5;
        float g = 5 <= 5.0;
        float h = 5 > 5;
        float i = 5 >= 5.0;
        float j = 5 == 5;
        float k = 5 != 5.0;
    })";

  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();

  ast->fold_expressions();

  // 5 + 5.0
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "10.000000");

  // 5.0 - 5
  auto stmt2 = ast->get_function("main")->body->get_statement(1);
  decl = dynamic_cast<ASTStmtDecl *>(stmt2);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0.000000");

  // 5 * 5.0
  auto stmt3 = ast->get_function("main")->body->get_statement(2);
  decl = dynamic_cast<ASTStmtDecl *>(stmt3);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "25.000000");

  // 5.0 / 5
  auto stmt4 = ast->get_function("main")->body->get_statement(3);
  decl = dynamic_cast<ASTStmtDecl *>(stmt4);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "1.000000");

  // 5 % 5.0
  auto stmt5 = ast->get_function("main")->body->get_statement(4);
  decl = dynamic_cast<ASTStmtDecl *>(stmt5);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_FLOAT_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "0.000000");

  // 5.0 < 5
  auto stmt6 = ast->get_function("main")->body->get_statement(5);
  decl = dynamic_cast<ASTStmtDecl *>(stmt6);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5 <= 5.0
  auto stmt7 = ast->get_function("main")->body->get_statement(6);
  decl = dynamic_cast<ASTStmtDecl *>(stmt7);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5 > 5
  auto stmt8 = ast->get_function("main")->body->get_statement(7);
  decl = dynamic_cast<ASTStmtDecl *>(stmt8);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");

  // 5 >= 5.0
  auto stmt9 = ast->get_function("main")->body->get_statement(8);
  decl = dynamic_cast<ASTStmtDecl *>(stmt9);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "true");

  // 5 == 5
  auto stmt10 = ast->get_function("main")->body->get_statement(9);
  decl = dynamic_cast<ASTStmtDecl *>(stmt10);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);

  // 5 != 5.0
  auto stmt11 = ast->get_function("main")->body->get_statement(10);
  decl = dynamic_cast<ASTStmtDecl *>(stmt11);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  ASSERT_EQ(expr->get_expr_type().primitive, KL_BOOL_PRIMITIVE);
  ASSERT_EQ(dynamic_cast<ASTExprConstantValue *>(expr)->value, "false");
}