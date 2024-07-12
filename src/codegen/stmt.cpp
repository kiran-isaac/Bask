#include "codegen.h"
#include "llvm/IR/Constants.h"

using namespace llvm;

CodeGenResult KLCodeGenVisitor::visit(ASTStmtExpr *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTStmtAssignment *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTStmtDecl *node) {}

CodeGenResult KLCodeGenVisitor::visit(ASTBlock *node) {}