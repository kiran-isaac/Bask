#include "codegen.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

CodeGenResult KLCodeGenVisitor::visit(ASTExprConstantValue *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTExprIdentifier *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTExprFuncCall *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTExprBinary *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTExprUnary *node) {}