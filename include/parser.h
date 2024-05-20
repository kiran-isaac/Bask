//
// Created by kiran on 5/20/24.
//

#ifndef KL_PARSER_H
#define KL_PARSER_H

#include <AST.h>
#include <lexer.h>
#include <types.h>
#include <queue>

class Parser {
private:
  Lexer &lexer;
  
  Token tk;
  queue<Token> peekQueue;
  
  Token peek(int n);
  void nextToken();
  void expect(KLTokenType type) const;
  
  static void parserError(const string &msg);
  
  unique_ptr<ASTFuncDecl> parseFunction();
  unique_ptr<ASTType> parseType();
  unique_ptr<ASTStmt> parseStatement();
  unique_ptr<ASTStmtExpr> parseExpressionStatement();
  unique_ptr<ASTStmt> parseAssignment();
  
  unique_ptr<ASTExpr> parseExpression();
  
  unique_ptr<ASTExprPrimary> parsePrimary();
  unique_ptr<ASTExprParen> parsePrimaryParens();
  unique_ptr<ASTExprFuncCall> parseFunctionCall();
public:
  explicit Parser(Lexer &lexer) : lexer(lexer) {}

  unique_ptr<ASTNode> parse();
  
};

#endif //KL_PARSER_H
