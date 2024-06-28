//
// Created by kiran on 5/20/24.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "AST/AST.h"
#include "parser.h"

const char *insertIntoTempFile(const char *contents) {
  char *tempFile = strdup("/tmp/kl-XXXXXX");
  mkstemp(tempFile);
  FILE *file = fopen(tempFile, "w");
  fprintf(file, "%s", contents);
  fclose(file);
  return tempFile;
}

std::unique_ptr<ASTProgram> parseTestProgram(const std::string &program) {
  {
    const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

    CommandLineArguments options(2, argv);
    Lexer lexer(options);
    Parser parser(lexer);
    return parser.parse();
  }
}