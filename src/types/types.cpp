//
// Created by kiran on 5/21/24.
//

#include "types.h"
#include <llvm/IR/LLVMContext.h>

string primitive_to_string(const KL_PrimitiveType &type) {
  switch (type)
  {
  case KL_INT_PRIMITIVE:
    return "int";
  case KL_FLOAT_PRIMITIVE:
    return "float";
  case KL_BOOL_PRIMITIVE:
    return "bool";
  case KL_CHAR_PRIMITIVE:
    return "char";
  case KL_STRING_PRIMITIVE:
    return "string";
  case KL_VOID_PRIMITIVE:
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
  case KL_PRIMITIVE_TYPEKIND:
    str += primitive_to_string(this->primitive);
    break;
  case KL_ARRAY_TYPEKIND:
    str += primitive_to_string(this->primitive);
    str += "[";
    for (auto &size : *this->array_sizes) {
      str += std::to_string(size);
      if (&size != &this->array_sizes->back()) str += ", ";
    }
    str += "]";
    break;
  case KL_FUNCTION_TYPEKIND:
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
  if (kind == KL_PRIMITIVE_TYPEKIND) {
      switch (primitive) {
        case KL_INT_PRIMITIVE:
          return llvm::Type::getInt32Ty(TheContext);
        case KL_FLOAT_PRIMITIVE:
          return llvm::Type::getFloatTy(TheContext);
        case KL_BOOL_PRIMITIVE:
          return llvm::Type::getInt1Ty(TheContext);
        case KL_CHAR_PRIMITIVE:
          return llvm::Type::getInt8Ty(TheContext);
        case KL_STRING_PRIMITIVE:
          return llvm::Type::getInt8PtrTy(TheContext);
        case KL_VOID_PRIMITIVE:
          return llvm::Type::getVoidTy(TheContext);
        default:
          return nullptr;
      }
  } else if (kind == KL_ARRAY_TYPEKIND) {
      auto depth = array_sizes->size();
      auto base_type = KL_Type(is_const, primitive).get_llvm_type(TheContext);
      auto array_type = llvm::ArrayType::get(base_type, (*array_sizes)[depth - 1]);
      for (int i = 1; i < depth; i++) {
        array_type = llvm::ArrayType::get(array_type, (*array_sizes)[depth - i - 1]);
      }
      return array_type;
  } else if (kind == KL_FUNCTION_TYPEKIND) {
      std::vector<llvm::Type *> params;
      for (auto it = signature->begin(); it != signature->end() - 1; ++it) {
        params.push_back(it->get_llvm_type(TheContext));
      }
      return llvm::FunctionType::get(signature->back().get_llvm_type(TheContext), params, false);
  }
  return nullptr;
}