#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/Casting.h>
#include <memory>
#include <random>
#include <vector>

#include "../utils/utils.h"
#include "AST/AST.h"
#include "codegen.h"
#include "types.h"

TEST(FuncDef, Main) {
  KLCodeGenVisitor visitor("Declarations.Main");

  const char *argv[] = {"KL",
                        insertIntoTempFile("int main() {}")};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  visitor.printModule();
}

TEST(FuncDef, MainWithArgs) {
  KLCodeGenVisitor visitor("Declarations.Main");

  const char *argv[] = {"KL", insertIntoTempFile("int main(int a, int b) {}")};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  visitor.printModule();
}