#include <climits>
#include <gtest/gtest.h>
#include <llvm/IR/Constants.h>
#include <random>

#include "../utils/utils.h"
#include "AST/AST.h"
#include "codegen.h"
#include "parser.h"
#include "types.h"

TEST(Expressions, Expr1) {
  string source = R"(
    int main() {  
      float x = 0.; 
      return 0;
    }
  )";
  
  compile_and_unpack("Main", source);

  auto main_func = get_IR_func_block(IR, "main").value();

  cout << main_func << endl;
}

TEST(Expressions, Expr2) {
  string source = R"(
    int main() {  
      float a = 0.; 
      float b = 1.0;
      float c = a + b;

      return 0;
    }
  )";

  compile_and_unpack("Main", source);

  auto main_func = get_IR_func_block(IR, "main").value();

  cout << main_func << endl;
}

TEST(Expressions, Expr3) {
  string source = R"(
    int main() {  
      float a = 0.; 
      float b = 1.0;
      float c = a + b;

      if (c >= 0) {
          return 1;
      } else {
          return 0;
      }
      return 0;
    }
  )";

  compile_and_unpack("Main", source);

  auto main_func = get_IR_func_block(IR, "main").value();

  cout << main_func << endl;
}