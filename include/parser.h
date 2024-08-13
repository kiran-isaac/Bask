//
// Created by kiran on 5/20/24.
//

#ifndef KL_PARSER_H
#define KL_PARSER_H

#include <queue>

#include "AST/AST.h"
#include "lexer.h"
#include "types.h"

class Parser {
 private:
  Lexer &lexer;

  KL_Token tk;
  queue<KL_Token> peekQueue;

  KL_Token peek(int n);
  void nextToken();
  void expect(KL_TokenType type) const;

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

 public:
  explicit Parser(Lexer &lexer) : lexer(lexer) { nextToken(); }

  unique_ptr<ASTProgram> parse();
};

#endif  // KL_PARSER_H
