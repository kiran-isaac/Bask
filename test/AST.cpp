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
  
  stringstream str;

  auto ast = parser.parse();
  auto stmt1 = ast->getFunction("main")->getStatement(0);
  auto decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  auto expr = dynamic_cast<ASTExpr *>(decl->value.get());
  printf("Before folding:\n");
  expr->print(0, str);
  
  ast->foldExpr();
  stmt1 = ast->getFunction("main")->getStatement(0);
  decl = dynamic_cast<ASTStmtDecl *>(stmt1);
  expr = dynamic_cast<ASTExpr *>(decl->value.get());
  printf("After folding:\n");
  expr->print(0, str);
}