#include <gtest/gtest.h>

#include "../utils/utils.h"
#include "AST/AST.h"
#include "parser.h"

TEST(DeclTypeCheck, Test1) {
  SYMTAB.wipe();
  auto ast = parseTestProgram(R"(
      int main() {
        int a = 0;
        int b = 1;
        int c = a + b;
      }
      )");

  ast->check_semantics();
  // Check that C is am int
  auto type = SYMTAB.get_name_type("c");
  ASSERT_EQ(type->to_string(), "int");
}

void testBadDecl(const std::string &program) {
  SYMTAB.wipe();
  EXPECT_THROW(parseTestProgram(program)->check_semantics(),
               std::runtime_error);
}

TEST(DeclTypeCheck, BadAssignment) {
  testBadDecl(R"(
  int main() {
    bool a = 0;
  })");

  testBadDecl(R"(
  int main() {
    int a = 0; 
    bool b = a;
  })");

  testBadDecl(R"(
  int main() {
    int a = 0; 
    int b = 1; 
    bool c = a + b;
  })");

  testBadDecl(R"(
  int main() {
    int a = 0; 
    const int b = 1; 
    b = a;
  })");

  testBadDecl(R"(
  int main() {
    int a = 0; 
    string b = "hello world"; 
    a = b;
  })");
}