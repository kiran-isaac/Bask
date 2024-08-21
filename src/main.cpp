#include "codegen.h"
#include <iostream>
#include <fstream>
#include <options.h>
#include <parser.h>
#include <lexer.h>

#include <llvm/Support/raw_os_ostream.h>

int main(int argc, const char **argv) {
  CommandLineArguments options(argc, argv);
  Lexer lexer(options);
  Parser parser(lexer);
  
  auto ast = parser.parse();
  // ast->fold_expressions();
  ast->check_semantics();

  ostream &out = options.out.empty() ? std::cout : *(new ofstream(options.out));

  if (options.mode == CommandLineArguments::Mode::AST) {
    ast->print(0, out);
    return 0;
  }

  BASKCodeGenVisitor visitor("Main");
  auto result = ast->accept(&visitor);

  if (result->getTypeOfResult() == CodeGenResultType_Error) {
    cerr << "Error: " << result->getError() << endl;
    return 1;
  }

  if (options.mode == CommandLineArguments::Mode::IR) {
    out << visitor.getModuleAsString();
  }

  if (options.mode == CommandLineArguments::Mode::COMPILE) {
    visitor.compileModule(options);
  }
}
