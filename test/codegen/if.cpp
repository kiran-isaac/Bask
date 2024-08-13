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

TEST(If, If1) {
  KLCodeGenVisitor visitor("Declarations.SimpleDeclaration");

  const char *argv[] = {"KL", insertIntoTempFile(R"(
      int main() { 
        int x = 0;
        if (true) {
          x = 1;
        } else {
          x = 2;
        }
        return x;
      })")};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  string output = visitor.getModuleAsString();

  cout << output << endl;
}