//
// Created by kiran on 5/20/24.
//

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <optional>
#include <vector>

#include "AST/AST.h"
#include "parser.h"
#include "utils.h"

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

string reformat(const string &str) {
  // replace all spaces with a single space
  string result = regex_replace(str, regex("\\s+"), " ");
  // remove leading and trailing spaces
  result = regex_replace(result, regex("^\\s+|\\s+$"), "");
  return result;
}


optional<string> get_IR_func_block(string &output, string func_name) {
  // Get position of function definition taking into account the different return type posibilities
  string next_func_name = "";
  size_t start = 0;

  while (next_func_name != func_name && start <= output.size()) {
    start = output.find("define ", start);
    if (start == string::npos) {
      return nullopt;
    }
    // Get name of the function declared on the line
    int next_func_name_start = output.find("@", start);
    if (next_func_name_start == string::npos) {
      return nullopt;
    }
    int next_func_name_end = output.find("(", next_func_name_start);
    if (next_func_name_end == string::npos) {
      return nullopt;
    }
    next_func_name = output.substr(next_func_name_start + 1, next_func_name_end - next_func_name_start - 1);
  }

  // find end of function, taking into account nested functions
  int count = 1;
  size_t end = output.find("{", start) + 1;

  while (count > 0 && end < output.size()) {
    if (output[end] == '{') {
      count++;
    } else if (output[end] == '}') {
      count--;
    }
    end++;
  }

  if (count != 0 || end >= output.size()) {
    return nullopt;
  }

  return output.substr(start, end - start);
}

string compile(string module_name, string program) {
  KLCodeGenVisitor visitor(module_name.c_str());

  const char *argv[] = {"KL", insertIntoTempFile(program.c_str())};

  CommandLineArguments options(2, argv);
  Lexer lexer(options);
  Parser parser(lexer);

  auto ast = parser.parse();
  ast->fold_expressions();

  ast->accept(&visitor);

  return visitor.getModuleAsString();
}