#include "tokens.h"
#include <parser.h>
#include <AST/AST.h>

unique_ptr<ASTControLFlowIf> Parser::parse_if() {
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
  
  return make_unique<ASTControLFlowIf>(std::move(condition), std::move(then_block), std::move(else_block), line, col);
}