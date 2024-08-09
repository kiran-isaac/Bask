#ifndef BB6A20A0_E738_4288_B176_043F7A743F89
#define BB6A20A0_E738_4288_B176_043F7A743F89

#include <iostream>
#include <llvm-14/llvm/IR/Type.h>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "codegen.h"
#include "symtab.h"
#include "tokens.h"
#include "types.h"

#define SYMTAB ASTNode::symtab

using namespace std;

void printIndent(int indent, ostream &out);

class ASTNode {
public:
  enum ASTNodeType {
    Type,

    Expr,
    ExprConstValue,
    ExprIdentifier,
    ExprFuncCall,
    ExprBinary,
    ExprUnary,

    Stmt,
    StmtExpr,
    StmtAssignment,
    StmtDecl,
    Block,

    ControlFlowIf,
    ControlFlowWhile,
    ControlFlowFor,
    ControlFlowReturn,
    ControlFlowContinue,
    ControlFlowBreak,
    Program,
    FuncDecl
  };

  static SymTab symtab;

  [[nodiscard]] virtual ASTNodeType get_AST_type() const = 0;

  unsigned int line;
  unsigned int col;

  virtual void fold_expressions() {}

  virtual void print(int indent, ostream &out) const = 0;

  virtual void check_semantics() {}

  virtual KLCodeGenResult *accept(KLCodeGenVisitor *v) = 0;

  static void SyntaxError(unsigned int line, unsigned int col,
                          const string &message) {
    string msg = "Syntax Error at [" + to_string(line) + ", " + to_string(col) +
                 "]: " + message;
    throw std::runtime_error(msg);
  }

  // Variable already exists etc
  static void ValueError(unsigned int line, unsigned int col,
                         const string &message) {
    string msg = "Value Error at [" + to_string(line) + ", " + to_string(col) +
                 "]: " + message;
    throw std::runtime_error(msg);
  }

  // Type mismatch etc
  static void TypeError(unsigned int line, unsigned int col,
                        const string &message) {
    string msg = "Type Error at [" + to_string(line) + ", " + to_string(col) +
                 "]: " + message;
    throw std::runtime_error(msg);
  }

  // TODO add warning functions
};

class ASTType : public ASTNode {
public:
  KL_Type type;
  unsigned int line;
  unsigned int col;

  explicit ASTType(KL_Type type, unsigned int line, unsigned int col)
      : type(std::move(type)), line(line), col(col) {}

  [[nodiscard]] ASTNodeType get_AST_type() const override { return Type; }

  KLCodeGenResult *accept(KLCodeGenVisitor *v) override { return v->visit(this); }

  void print(int indent, ostream &out) const override {
    printIndent(indent, out);
    out << type.to_string() << std::endl;
  }

  llvm::Type *to_llvm_type();
};

class ASTProgram;

#endif /* BB6A20A0_E738_4288_B176_043F7A743F89 */