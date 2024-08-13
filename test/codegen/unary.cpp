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

TEST(Unary, UnaryNegDeclaration) { 
  KLCodeGenVisitor visitor("Declarations.SimpleDeclaration");

  const char *argv[] = {"KL",
                        insertIntoTempFile("int main() { int a = -5; int b = -a; }")};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  string output = visitor.getModuleAsString();

  cout << output << endl;

  // string expected_main =
  //     R"(
  //   define i32 @main() {
  //   entry:
  //     %a = alloca i32, align 4
  //     store i32 50, i32* %a, align 4
  //   }
  //   )";

  // auto main_func = get_IR_func_block(output, "main").value();

  // EXPECT_EQ(reformat(main_func), reformat(expected_main));
}