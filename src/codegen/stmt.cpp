#include "codegen.h"
#include <llvm/IR/Constants.h>

using namespace llvm;

KLCodeGenResult *KLCodeGenVisitor::visit(ASTStmtExpr *node) {}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTStmtAssignment *node) {}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTStmtDecl *node) {}

KLCodeGenResult *KLCodeGenVisitor::visit(ASTBlock *node) {}