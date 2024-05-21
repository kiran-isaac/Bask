//
// Created by kiran on 5/21/24.
//

#ifndef KL_AST_CONTROLFLOW_H
#define KL_AST_CONTROLFLOW_H

#include "AST_Statements.h"
#include "AST_Expressions.h"
#include "AST.h"

class ASTControLFlowIf : ASTStmt {
  unique_ptr<ASTExpr> condition;
  unique_ptr<ASTStmt> then_block;
  unique_ptr<ASTStmt> else_block;
  
  [[nodiscard]] ASTNodeType get_AST_type() const override {
    return ControlFlowIf;
  }
  
  ASTControLFlowIf(unique_ptr<ASTExpr> condition, unique_ptr<ASTStmt> then_block, unique_ptr<ASTStmt> else_block)
    : condition(std::move(condition)), then_block(std::move(then_block)), else_block(std::move(else_block)) {}
};

#endif //KL_AST_CONTROLFLOW_H
