//
// Created by kiran on 5/20/24.
//

#ifndef KL_UTILS_H
#define KL_UTILS_H

#include <regex>

#include "parser.h"

const char *insertIntoTempFile(const char *contents);

string reformat(const string &str);

optional<string> get_IR_func_block(string &output, string func_name);

unique_ptr<ASTProgram> parseTestProgram(const string &program);

#endif //KL_UTILS_H
