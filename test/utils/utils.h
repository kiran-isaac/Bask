//
// Created by kiran on 5/20/24.
//

#ifndef BASK_UTILS_H
#define BASK_UTILS_H

#include <regex>
#include <string>

#include "parser.h"

const char *insertIntoTempFile(const char *contents);

string reformat(const string &str);

optional<string> get_IR_func_block(string &output, string func_name);

unique_ptr<ASTProgram> parseTestProgram(const string &program);

pair<bool, string> compile(string module_name, string program);

#define compile_and_unpack(module_name, program) \
  auto result = compile(module_name, program); \
  if (!result.first) { \
    FAIL() << result.second; \
  } \
  auto IR = result.second;

#endif //BASK_UTILS_H
