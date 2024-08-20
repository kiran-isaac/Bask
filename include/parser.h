//
// Created by kiran on 5/20/24.
//

#ifndef BASK_PARSER_H
#define BASK_PARSER_H

#include <queue>

#include "AST/AST.h"
#include "codegen.h"
#include "lexer.h"
#include "types.h"

class Parser {
 private:
  Lexer &lexer;

  BASK_Token tk;
  queue<BASK_Token> peekQueue;

  BASK_Token peek(int n);
  void nextToken();
  void expect(BASK_TokenType type) const;

  void parserError(const string &msg) const;

  // program
  unique_ptr<ASTFuncDecl> parse_function();
  unique_ptr<ASTType> parse_type_annotation();
  unique_ptr<ASTBlock> parse_block();

  // statements
  unique_ptr<ASTStmt> parse_statement();
  unique_ptr<ASTStmtExpr> parse_expression_statement();
  unique_ptr<ASTStmtAssignment> parse_assignment();
  unique_ptr<ASTStmtDecl> parse_declaration();
  unique_ptr<ASTStmtReturn> parse_return();

  // expressions
  unique_ptr<ASTExpr> parse_expression();
  unique_ptr<ASTExpr> parse_binary_expression(unique_ptr<ASTExpr> LHS,
                                              int min_precedence);
  unique_ptr<ASTExpr> parse_unary_expression();
  unique_ptr<ASTExpr> parse_primary();
  unique_ptr<ASTExpr> parse_primary_parens();
  unique_ptr<ASTExprFuncCall> parse_function_call();

  unique_ptr<ASTControlFlowIf> parse_if();
  unique_ptr<ASTControlFlowWhile> parse_while();
public:
  explicit Parser(Lexer &lexer) : lexer(lexer) { nextToken(); }

  unique_ptr<ASTProgram> parse();
};

#endif  // BASK_PARSER_H
