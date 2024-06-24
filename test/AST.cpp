//
// Created by kiran on 5/21/24.
//

#include "AST/AST.h"
#include <gtest/gtest.h>
#include <memory>
#include <parser.h>
#include <sstream>
#include "utils/utils.h"

using namespace std;

TEST(ASTFolding, IntegerAdd) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = a + print(10 + 11); }")};
  
  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  auto stmt1 = ast->get_function("main")->body->get_statement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  expr->print(0, cout);
  
  ast->fold_expressions();
  stmt1 = ast->get_function("main")->body->get_statement(0);
  decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  expr->print(0, cout);
}

TEST(ASTFolding, String) {
  string program =
    R"(int main() {
        string d = "Hello, " + "World!" + '\n';
        string a = "Hello, " + "World!";
        bool b = "Hello, " == "World!";
        bool c = "Hello, " != "World!";
    })";
  const char* argv[] = {"KL", insertIntoTempFile(program.c_str())};
  
  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();
  printf("Before folding:\n");
  ast->print(0, cout);
  
  ast->fold_expressions();
  ast->print(0, cout);
}