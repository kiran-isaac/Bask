//
// Created by kiran on 5/20/24.
//

#include <gtest/gtest.h>
#include <memory>
#include <parser.h>
#include "utils/utils.h"

using namespace std;

TEST(Parser, Parse) {
  const char* argv[] = {"KL", insertIntoTempFile("int main() { return 0; }")};
  
  Options options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
}