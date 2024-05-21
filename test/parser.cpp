//
// Created by kiran on 5/20/24.
//

#include <gtest/gtest.h>
#include <memory>
#include <parser.h>
#include "utils/utils.h"

using namespace std;

TEST(Parser, Parse) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { return 0; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
}

TEST(Parser, Assignment) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = 5; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
}

TEST(Parser, FunctionCall) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { print(\"Hello, World!\"); }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->print(0);
}

TEST(Parser, TinyBinaryExpression) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = 5 + 3; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->print(0);
}

TEST(Parser, BinaryExpression) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = -(5 + 3) * echo(2); }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->getFunction("main")->getStatement(0)->getExpr()->print(0);
}

TEST(Parser, BinaryExpressionFile) {
  const char* argv[] = {"KL", "../test/sample/binary_expressions.kl"};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  unique_ptr<ASTProgram> ast = parser.parse();
  ast->getFunction("main")->getStatement(2)->print(0);
}