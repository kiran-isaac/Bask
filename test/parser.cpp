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
  
  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
}

TEST(Parser, Declaration) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = 5 * 10; }")};
  
  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->print(0, std::cout);
}

TEST(Parser, Assignment) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { a = 5 * 10; }")};
  
  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->fold_expressions();
  
  auto stmt = ast->get_function("main")->body->get_statement(0);
  auto assignment = dynamic_cast<ASTStmtAssignment *>(stmt);
  auto expr = dynamic_cast<ASTExpr *>(assignment->value.get());
  auto value = dynamic_cast<ASTExprConstantValue *>(expr);
  
  ASSERT_NE(value, nullptr);
  
  ASSERT_EQ(assignment->name, "a");
  ASSERT_EQ(value->value, "50");
}

TEST(Parser, FunctionCall) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { print(\"Hello, World!\"); }")};
  
  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  auto stmt = ast->get_function("main")->body->get_statement(0);
  auto expr = dynamic_cast<ASTExprFuncCall *>(dynamic_cast<ASTStmtExpr *>(stmt)->expr.get());
  
  ASSERT_NE(expr, nullptr);
  
  ASSERT_EQ(expr->name, "print");
  ASSERT_EQ(expr->args->size(), 1);
  
  auto arg = dynamic_cast<ASTExprConstantValue *>(expr->args->at(0).get());
  ASSERT_NE(arg, nullptr);
  ASSERT_EQ(arg->value, "Hello, World!");
}