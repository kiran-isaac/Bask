#include <climits>
#include <gtest/gtest.h>
#include <llvm/IR/Constants.h>
#include <random>

#include "../utils/utils.h"
#include "AST/AST.h"
#include "codegen.h"
#include "parser.h"
#include "types.h"

TEST(Expression, Expr1) {
  string source = R"(
    int main() {  
      float x = 0.; 
    }
  )";
  string IR = compile("Declarations.SimpleDeclaration", source);

  auto main_func = get_IR_func_block(IR, "main").value();

  cout << main_func << endl;
}