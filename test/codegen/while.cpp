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

TEST(While, While1) {
  KLCodeGenVisitor visitor("Declarations.SimpleDeclaration");

  const char *argv[] = {"KL", insertIntoTempFile(R"(
      int main() { 
        int x = 0;
        while (x < 10) {
          x = x + 1;
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