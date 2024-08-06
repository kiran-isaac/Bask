#include <climits>
#include <gtest/gtest.h>
#include <llvm/IR/Constants.h>
#include <random>

#include "AST/AST.h"
#include "codegen.h"
#include "types.h"

TEST(CodeGen, VisitConstantInt) {
  KLCodeGenVisitor visitor("CodeGen.VistConstantInt");
  ASTExprConstantValue node(INT_CONST, "Replace", 0, 0);

  for (int i = 0; i < 1000; i++) {
    // Generate a random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(INT_MIN, INT_MAX);

    int value = dis(gen);
    node.value = std::to_string(value);

    auto result = visitor.visit(&node);

    ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
    ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result->getValue()));
    ASSERT_EQ(llvm::cast<llvm::ConstantInt>(result->getValue())->getSExtValue(), value);
  }
}

TEST(CodeGen, VisitConstantFloat) {
  KLCodeGenVisitor visitor("CodeGen.VisitConstantFloat");
  ASTExprConstantValue node(FLOAT_CONST, "42.0", 0, 0);

  for (int i = 0; i < 1000; i++) {
    // Generate a random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(FLT_MIN, FLT_MAX);

    float value = dis(gen);
    node.value = std::to_string(value);

    auto result = visitor.visit(&node);

    ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
    ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result->getValue()));
    ASSERT_EQ(llvm::cast<llvm::ConstantFP>(result->getValue())->getValueAPF().convertToFloat(), value);
  }
}

TEST(CodeGen, VisitConstantBool) {
  KLCodeGenVisitor visitor("CodeGen.VisitConstantBool");
  ASTExprConstantValue node(BOOL_CONST, "true", 0, 0);

  auto result = visitor.visit(&node);

  ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result->getValue()));
  ASSERT_EQ(llvm::cast<llvm::ConstantInt>(result->getValue())->getZExtValue(), 1);

  node.value = "false";
  result = visitor.visit(&node);

  ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
  ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result->getValue()));
  ASSERT_EQ(llvm::cast<llvm::ConstantInt>(result->getValue())->getZExtValue(), 0);
}

TEST(CodeGen, VisitConstantChar) {
  KLCodeGenVisitor visitor("CodeGen.VisitConstantChar");
  ASTExprConstantValue node(CHAR_CONST, "a", 0, 0);

  for (unsigned char i = 0; i < 255; i++) {
    node.value = std::string(1, i);
    auto result = visitor.visit(&node);

    ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
    ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result->getValue()));
    ASSERT_EQ(llvm::cast<llvm::ConstantInt>(result->getValue())->getZExtValue(), i);
  }
}

TEST(CodeGen, VisitConstantString) {
  KLCodeGenVisitor visitor("CodeGen.VisitConstantString");
  ASTExprConstantValue node(STRING_CONST, "Hello, World!", 0, 0);

  auto result = visitor.visit(&node);

  ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
  ASSERT_TRUE(llvm::isa<llvm::ConstantDataArray>(result->getValue()));
  ASSERT_EQ(llvm::cast<llvm::ConstantDataArray>(result->getValue())->getAsCString(), "Hello, World!");
}