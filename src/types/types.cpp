//
// Created by kiran on 5/21/24.
//

#include "types.h"
#include <llvm-14/llvm/IR/LLVMContext.h>

string primitive_to_string(const KL_PrimitiveType &type) {
  switch (type)
  {
  case KL_INT:
    return "int";
  case KL_FLOAT:
    return "float";
  case KL_BOOL:
    return "bool";
  case KL_CHAR:
    return "char";
  case KL_STRING:
    return "string";
  case KL_VOID:
    return "void";
  default:
    return "";
  }
}

string KL_Type::to_string() const {
  string str;
  if (this->is_const) {
    str += "const ";
  }
  
  switch (this->kind)
  {
  case KL_PRIMITIVE:
    str += primitive_to_string(this->primitive);
    break;
  case KL_ARRAY:
    str += primitive_to_string(this->primitive);
    str += "[";
    for (auto &size : *this->array_sizes) {
      str += std::to_string(size);
      if (&size != &this->array_sizes->back()) str += ", ";
    }
    str += "]";
    break;
  case KL_FUNCTION:
    str += "(";
    for (auto &param : *this->signature) {
      str += param.to_string();
      if (&param != &this->signature->back()) str += " -> ";
    }
    str += ")";
  default:
    break;
  }

  
  return str;
}

llvm::Type *KL_Type::get_llvm_type(llvm::LLVMContext &TheContext) const {
  if (kind == KL_PRIMITIVE) {
      switch (primitive) {
        case KL_INT:
          return llvm::Type::getInt32Ty(TheContext);
        case KL_FLOAT:
          return llvm::Type::getFloatTy(TheContext);
        case KL_BOOL:
          return llvm::Type::getInt1Ty(TheContext);
        case KL_CHAR:
          return llvm::Type::getInt8Ty(TheContext);
        case KL_STRING:
          return llvm::Type::getInt8PtrTy(TheContext);
        case KL_VOID:
          return llvm::Type::getVoidTy(TheContext);
        default:
          return nullptr;
      }
  } else if (kind == KL_ARRAY) {
      auto depth = array_sizes->size();
      auto base_type = KL_Type(is_const, primitive).get_llvm_type(TheContext);
      auto array_type = llvm::ArrayType::get(base_type, (*array_sizes)[depth - 1]);
      for (int i = 1; i < depth; i++) {
        array_type = llvm::ArrayType::get(array_type, (*array_sizes)[depth - i - 1]);
      }
      return array_type;
  } else if (kind == KL_FUNCTION) {
      std::vector<llvm::Type *> params;
      for (auto &param : *signature) {
        params.push_back(param.get_llvm_type(TheContext));
      }
      return llvm::FunctionType::get(signature->back().get_llvm_type(TheContext), params, false);
  }
  return nullptr;
}