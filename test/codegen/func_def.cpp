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
  BASKCodeGenVisitor visitor("Declarations.Main");

  const char *argv[] = {"BASK",
                        insertIntoTempFile("int main() {return 0;}")};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  visitor.printModule();
}

TEST(FuncDef, MainWithArgs) {
  BASKCodeGenVisitor visitor("Declarations.Main");

  const char *argv[] = {"BASK", insertIntoTempFile("int main(int a, int b) {return 0;}")};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  // create llvm output stream
  std::string output;

  llvm::raw_string_ostream llvm_output(output);

  visitor.printModule(llvm_output);

  std::string expected_output =
      R"(; ModuleID = 'Declarations.Main'
    source_filename = "Declarations.Main"
    target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

    define i64 @main(i64 %a, i64 %b) {
    entry:
      ret i64 0
    }
  )";

  EXPECT_EQ(reformat(output), reformat(expected_output));
}