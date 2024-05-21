//
// Created by kiran on 5/20/24.
//

#include <gtest/gtest.h>
#include <memory>
#include <parser.h>
#include "utils/utils.h"

using namespace std;

TEST(Parser, ParseFunction) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() {  }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
}

TEST(Parser, Declaration) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = 5 * 10; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->print(0, std::cout);
}

TEST(Parser, Assignment) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { a = 5 * 10; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->fold_expressions();
  
  auto stmt = ast->get_function("main")->body->get_statement(0);
  auto assignment = dynamic_cast<ASTStmtAssignment *>(stmt);
  auto expr = dynamic_cast<ASTExpr *>(assignment->value.get());
  auto value = dynamic_cast<ASTExprValue *>(expr);
  
  ASSERT_NE(value, nullptr);
  
  ASSERT_EQ(assignment->name, "a");
  ASSERT_EQ(value->value, "50");
}

TEST(Parser, FunctionCall) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { print(\"Hello, World!\"); }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  auto stmt = ast->get_function("main")->body->get_statement(0);
  auto expr = dynamic_cast<ASTExprFuncCall *>(dynamic_cast<ASTStmtExpr *>(stmt)->expr.get());
  
  ASSERT_NE(expr, nullptr);
  
  ASSERT_EQ(expr->name, "print");
  ASSERT_EQ(expr->args->size(), 1);
  
  auto arg = dynamic_cast<ASTExprValue *>(expr->args->at(0).get());
  ASSERT_NE(arg, nullptr);
  ASSERT_EQ(arg->value, "Hello, World!");
}

TEST(Parser, TinyBinaryExpression) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = 5 + 3; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->print(0, std::cout);
}

TEST(Parser, BinaryExpression) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = -(5 + 3) * echo(2); }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
}

TEST(Parser, BinaryExpression2) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = 5 + 3 * -(2 + 10); }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  unique_ptr<ASTProgram> ast = parser.parse();
  ast->get_function("main")->body->get_statement(0)->print(0, std::cout);
}

