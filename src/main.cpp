#include "codegen.h"
#include <iostream>
#include <options.h>
#include <parser.h>
#include <lexer.h>

int main(int argc, const char **argv) {
  CommandLineArguments options(argc, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  ast->fold_expressions();
  ast->check_semantics();

  KLCodeGenVisitor visitor("Main");
  ast->accept(&visitor);

  visitor.printModule();
}
