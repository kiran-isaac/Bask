#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/Casting.h>
#include <random>
#include <vector>

#include "AST/AST.h"
#include "codegen.h"
#include "types.h"

TEST(TypeToLLVMType, PrimitiveTypes) {
  BASKCodeGenVisitor visitor("TypeToLLVMType.PrimitiveTypes");

  ASTType type(BASK_Type(), 0, 0);

  llvm::SmallVector<llvm::Type::TypeID, 6> expected_results = {
      llvm::Type::TypeID::IntegerTyID, llvm::Type::TypeID::DoubleTyID,
      llvm::Type::TypeID::IntegerTyID, llvm::Type::TypeID::IntegerTyID,
      llvm::Type::TypeID::PointerTyID, llvm::Type::TypeID::VoidTyID};

  auto expected_results_it = expected_results.begin();
  for (auto type : {BASK_INT_PRIMITIVE, BASK_FLOAT_PRIMITIVE, BASK_BOOL_PRIMITIVE, BASK_CHAR_PRIMITIVE, BASK_STRING_PRIMITIVE, BASK_VOID_PRIMITIVE}) {
    BASK_Type bask_type;
    bask_type.kind = BASK_PRIMITIVE_TYPEKIND;
    bask_type.primitive = type;

    ASTType type_node(bask_type, 0, 0);

    auto llvm_type_result = type_node.accept(&visitor);

    assert(llvm_type_result->getTypeOfResult() == CodeGenResultType_Type);

    auto llvm_type = llvm_type_result->getLLVMType();

    assert(llvm::isa<llvm::Type>(llvm_type));

    assert(llvm_type->getTypeID() == *expected_results_it);
    expected_results_it = std::next(expected_results_it);
  }
}

TEST(TypeToLLVMType, ArrayTypes) {
  BASKCodeGenVisitor visitor("TypeToLLVMType.ArrayTypes");

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

    ASTType type_node(BASK_Type(false, BASK_INT_PRIMITIVE, array_sizes), 0, 0);

    BASKCodeGenResult *llvm_type_result = type_node.accept(&visitor);
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
  BASKCodeGenVisitor visitor("TypeToLLVMType.FunctionType");

  auto *signature = new std::vector<BASK_Type>();
  signature->push_back(BASK_Type(false, BASK_FLOAT_PRIMITIVE));
  signature->push_back(BASK_Type(false, BASK_INT_PRIMITIVE));
  signature->push_back(BASK_Type(false, BASK_INT_PRIMITIVE));

  BASK_Type bask_type(true, signature);

  ASTType type_node(bask_type, 0, 0);

  BASKCodeGenResult *llvm_type_result = type_node.accept(&visitor);
  assert(llvm_type_result->getTypeOfResult() == CodeGenResultType_Type);

  llvm::Type *llvm_type = llvm_type_result->getLLVMType();
  assert(llvm::isa<llvm::Type>(llvm_type));
  assert(llvm_type->getTypeID() == llvm::Type::TypeID::FunctionTyID);

  llvm::FunctionType *function_type = llvm::cast<llvm::FunctionType>(llvm_type);

  assert(function_type->getReturnType()->getTypeID() == llvm::Type::TypeID::IntegerTyID);
  assert(function_type->getNumParams() == 2);
  assert(function_type->getParamType(0)->getTypeID() == llvm::Type::TypeID::DoubleTyID);
  assert(function_type->getParamType(1)->getTypeID() == llvm::Type::TypeID::IntegerTyID);
}