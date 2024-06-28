//
// Created by kiran on 5/20/24.
//

#ifndef KL_UTILS_H
#define KL_UTILS_H

#include "parser.h"

const char *insertIntoTempFile(const char *contents);

std::unique_ptr<ASTProgram> parseTestProgram(const std::string &program);

#endif //KL_UTILS_H
