#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/Casting.h>
#include <memory>
#include <random>
#include <vector>

#include "AST/AST.h"
#include "codegen.h"
#include "types.h"
#include "../utils/utils.h"

// TEST(Declarations, SimpleDeclaration) {
//   KLCodeGenVisitor visitor("Declarations.SimpleDeclaration");

//   const char *argv[] = {"KL",
//                         insertIntoTempFile("int main() { int a = 5 * 10; }")};

//   CommandLineArguments options(2, argv);
//   Lexer lexer(options);
//   Parser parser(lexer);

//   auto ast = parser.parse();
//   ast->fold_expressions();

//   auto stmt = ast->get_function("main")->body->get_statement(0);
//   auto assignment = dynamic_cast<ASTStmtDecl *>(stmt);
//   auto expr = dynamic_cast<ASTExpr *>(assignment->value.get());

//   auto result = visitor.visit(assignment);
// }