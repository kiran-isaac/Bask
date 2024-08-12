#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <llvm-14/llvm/Support/Casting.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <random>
#include <vector>

#include "AST/AST.h"
#include "codegen.h"
#include "types.h"

TEST(TypeToLLVMType, PrimitiveTypes) {
  KLCodeGenVisitor visitor("TypeToLLVMType.PrimitiveTypes");

  ASTType type(KL_Type(), 0, 0);

  llvm::SmallVector<llvm::Type::TypeID, 6> expected_results = {
      llvm::Type::TypeID::IntegerTyID, llvm::Type::TypeID::FloatTyID,
      llvm::Type::TypeID::IntegerTyID, llvm::Type::TypeID::IntegerTyID,
      llvm::Type::TypeID::PointerTyID, llvm::Type::TypeID::VoidTyID};

  auto expected_results_it = expected_results.begin();
  for (auto type : {KL_INT, KL_FLOAT, KL_BOOL, KL_CHAR, KL_STRING, KL_VOID}) {
    KL_Type kl_type;
    kl_type.kind = KL_PRIMITIVE;
    kl_type.primitive = type;

    ASTType type_node(kl_type, 0, 0);

    auto llvm_type_result = type_node.accept(&visitor);

    assert(llvm_type_result->getTypeOfResult() == CodeGenResultType_Type);

    auto llvm_type = llvm_type_result->getLLVMType();

    assert(llvm::isa<llvm::Type>(llvm_type));

    assert(llvm_type->getTypeID() == *expected_results_it);
    expected_results_it = std::next(expected_results_it);
  }
}

TEST(TypeToLLVMType, ArrayTypes) {
  KLCodeGenVisitor visitor("TypeToLLVMType.ArrayTypes");

  // Generate a random number
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, INT_MAX);

  for (int i = 1; i < 20; i++) {
    // Generate random array sizes
    std::vector<unsigned int> array_sizes;
    for (int j = 0; j < i; j++) {
      array_sizes.push_back(dis(gen));
    }

    ASTType type_node(KL_Type(false, KL_INT, array_sizes), 0, 0);

    KLCodeGenResult *llvm_type_result = type_node.accept(&visitor);
    assert(llvm_type_result->getTypeOfResult() == CodeGenResultType_Type);

    llvm::Type *llvm_type = llvm_type_result->getLLVMType();
    assert(llvm::isa<llvm::Type>(llvm_type));
    assert(llvm_type->getTypeID() == llvm::Type::TypeID::ArrayTyID);

    llvm::Type *array_type = llvm::cast<llvm::ArrayType>(llvm_type);

    while (array_type->isArrayTy()) {
      llvm::ArrayType *array_type_casted = llvm::cast<llvm::ArrayType>(array_type);
      assert(array_type_casted->getNumElements() == array_sizes.front());
      array_sizes.erase(array_sizes.begin());
      array_type = array_type_casted->getElementType();
    }
  }
}

TEST(TypeToLLVMType, FunctionType) {
  KLCodeGenVisitor visitor("TypeToLLVMType.FunctionType");

  auto *signature = new std::vector<KL_Type>();
  signature->push_back(KL_Type(false, KL_FLOAT));
  signature->push_back(KL_Type(false, KL_INT));
  signature->push_back(KL_Type(false, KL_INT));

  KL_Type kl_type(true, signature);

  ASTType type_node(kl_type, 0, 0);

  KLCodeGenResult *llvm_type_result = type_node.accept(&visitor);
  assert(llvm_type_result->getTypeOfResult() == CodeGenResultType_Type);

  llvm::Type *llvm_type = llvm_type_result->getLLVMType();
  assert(llvm::isa<llvm::Type>(llvm_type));
  assert(llvm_type->getTypeID() == llvm::Type::TypeID::FunctionTyID);

  llvm::FunctionType *function_type = llvm::cast<llvm::FunctionType>(llvm_type);

  assert(function_type->getReturnType()->getTypeID() == llvm::Type::TypeID::IntegerTyID);
  assert(function_type->getNumParams() == 2);
  assert(function_type->getParamType(0)->getTypeID() == llvm::Type::TypeID::FloatTyID);
  assert(function_type->getParamType(1)->getTypeID() == llvm::Type::TypeID::IntegerTyID);
}