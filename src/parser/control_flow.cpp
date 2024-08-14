#include "tokens.h"
#include <parser.h>
#include <AST/AST.h>

unique_ptr<ASTControlFlowIf> Parser::parse_if() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  nextToken();
  auto condition = parse_expression();
  auto then_block = parse_block();
  unique_ptr<ASTBlock> else_block = nullptr;
  
  if (tk.type == KL_TT_KW_Else) {
    nextToken();
    else_block = parse_block();
  }
  
  return make_unique<ASTControlFlowIf>(std::move(condition), std::move(then_block), std::move(else_block), line, col);
}

unique_ptr<ASTControlFlowWhile> Parser::parse_while() {
  unsigned int line = tk.line;
  unsigned int col = tk.col;
  
  nextToken();
  auto condition = parse_expression();
  auto body = parse_block();
  
  return make_unique<ASTControlFlowWhile>(std::move(condition), std::move(body), line, col);
}