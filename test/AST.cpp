//
// Created by kiran on 5/21/24.
//

#include <AST.h>
#include <gtest/gtest.h>
#include <memory>
#include <parser.h>
#include <sstream>
#include "utils/utils.h"

using namespace std;

TEST(ASTFolding, IntegerAdd) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { int a = a + 10 + 10; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  auto stmt1 = ast->getFunction("main")->getStatement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  expr->print(0, cout);
  
  ast->foldExpressions();
  stmt1 = ast->getFunction("main")->getStatement(0);
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
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  auto ast = parser.parse();
  printf("Before folding:\n");
  ast->print(0, cout);
  
  ast->foldExpressions();
  ast->print(0, cout);
}