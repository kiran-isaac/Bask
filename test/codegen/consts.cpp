#include <climits>
#include <gtest/gtest.h>
#include <llvm/IR/Constants.h>
#include <random>

#include "AST/AST.h"
#include "codegen.h"
#include "types.h"
#include "../utils/utils.h"
#include "parser.h"

TEST(CodeGen, VisitConstantInt) {
  BASKCodeGenVisitor visitor("CodeGen.VistConstantInt");
  ASTExprConstantValue node(BASK_INT_CONST, "Replace", 0, 0);
  
  // Generate a random number
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(INT_MIN, INT_MAX);

  for (int i = 0; i < 1000; i++) {
    int value = dis(gen);
    node.value = std::to_string(value);

    auto result = visitor.visit(&node);

    ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
    ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result->getValue()));
    ASSERT_EQ(llvm::cast<llvm::ConstantInt>(result->getValue())->getSExtValue(), value);
  }
}

TEST(CodeGen, VisitConstantFloat) {
  BASKCodeGenVisitor visitor("CodeGen.VisitConstantFloat");
  ASTExprConstantValue node(BASK_FLOAT_CONST, "42.0", 0, 0);

  // Generate a random number
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(FLT_MIN, FLT_MAX);

  for (int i = 0; i < 1000; i++) {
    float value = dis(gen);
    node.value = std::to_string(value);

    auto result = visitor.visit(&node);

    ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
    ASSERT_TRUE(llvm::isa<llvm::ConstantFP>(result->getValue()));
    ASSERT_EQ(llvm::cast<llvm::ConstantFP>(result->getValue())->getValueAPF().convertToFloat(), value);
  }
}

TEST(CodeGen, VisitConstantBool) {
  BASKCodeGenVisitor visitor("CodeGen.VisitConstantBool");
  ASTExprConstantValue node(BASK_BOOL_CONST, "true", 0, 0);

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
  BASKCodeGenVisitor visitor("CodeGen.VisitConstantChar");
  ASTExprConstantValue node(BASK_CHAR_CONST, "a", 0, 0);

  for (unsigned char i = 0; i < 255; i++) {
    node.value = std::string(1, i);
    auto result = visitor.visit(&node);

    ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
    ASSERT_TRUE(llvm::isa<llvm::ConstantInt>(result->getValue()));
    ASSERT_EQ(llvm::cast<llvm::ConstantInt>(result->getValue())->getZExtValue(), i);
  }
}

TEST(CodeGen, VisitConstantString) {
  BASKCodeGenVisitor visitor("CodeGen.VisitConstantString");
  ASTExprConstantValue node(BASK_STRING_CONST, "Hello, World!", 0, 0);

  auto result = visitor.visit(&node);

  ASSERT_EQ(result->getTypeOfResult(), CodeGenResultType_Value);
  ASSERT_TRUE(llvm::isa<llvm::ConstantDataArray>(result->getValue()));
  ASSERT_EQ(llvm::cast<llvm::ConstantDataArray>(result->getValue())->getAsCString(), "Hello, World!");
}

TEST(CodeGen, FloatConst) {
  string source = R"(
    int main() {  
      float x = 0.; 
      return 0;
    }
  )";

  compile_and_unpack("Declarations.SimpleDeclaration", source);

  auto main_func = get_IR_func_block(IR, "main").value();

  cout << main_func << endl;
}