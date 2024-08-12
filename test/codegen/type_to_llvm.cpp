#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <llvm-14/llvm/Support/Casting.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

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

  for (int i = 1; i < 20; i++) {
    // vector of i random numbers
    std::vector<unsigned int> array_sizes;
    for (int j = 0; j < i; j++) {
      array_sizes.push_back(j + 1);
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
      cout << array_type_casted->getNumElements() << " " << array_sizes.front() << endl;
      assert(array_type_casted->getNumElements() == array_sizes.front());
      array_sizes.erase(array_sizes.begin());
      array_type = array_type_casted->getElementType();
    }
  }
}