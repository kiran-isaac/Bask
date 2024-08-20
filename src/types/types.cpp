//
// Created by kiran on 5/21/24.
//

#include "types.h"
#include <llvm/IR/LLVMContext.h>

string primitive_to_string(const BASK_PrimitiveType &type) {
  switch (type)
  {
  case BASK_INT_PRIMITIVE:
    return "int";
  case BASK_FLOAT_PRIMITIVE:
    return "float";
  case BASK_BOOL_PRIMITIVE:
    return "bool";
  case BASK_CHAR_PRIMITIVE:
    return "char";
  case BASK_STRING_PRIMITIVE:
    return "string";
  case BASK_VOID_PRIMITIVE:
    return "void";
  default:
    return "";
  }
}

string BASK_Type::to_string() const {
  string str;
  if (this->is_const) {
    str += "const ";
  }
  
  switch (this->kind)
  {
  case BASK_PRIMITIVE_TYPEKIND:
    str += primitive_to_string(this->primitive);
    break;
  case BASK_ARRAY_TYPEKIND:
    str += primitive_to_string(this->primitive);
    str += "[";
    for (auto &size : *this->array_sizes) {
      str += std::to_string(size);
      if (&size != &this->array_sizes->back()) str += ", ";
    }
    str += "]";
    break;
  case BASK_FUNCTION_TYPEKIND:
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

llvm::Type *BASK_Type::get_llvm_type(llvm::LLVMContext &TheContext) const {
  if (kind == BASK_PRIMITIVE_TYPEKIND) {
      switch (primitive) {
        case BASK_INT_PRIMITIVE:
          return llvm::Type::getInt64Ty(TheContext);
        case BASK_FLOAT_PRIMITIVE:
          return llvm::Type::getDoubleTy(TheContext);
        case BASK_BOOL_PRIMITIVE:
          return llvm::Type::getInt1Ty(TheContext);
        case BASK_CHAR_PRIMITIVE:
          return llvm::Type::getInt8Ty(TheContext);
        case BASK_STRING_PRIMITIVE:
          return llvm::Type::getInt8PtrTy(TheContext);
        case BASK_VOID_PRIMITIVE:
          return llvm::Type::getVoidTy(TheContext);
        default:
          return nullptr;
      }
  } else if (kind == BASK_ARRAY_TYPEKIND) {
      auto depth = array_sizes->size();
      auto base_type = BASK_Type(is_const, primitive).get_llvm_type(TheContext);
      auto array_type = llvm::ArrayType::get(base_type, (*array_sizes)[depth - 1]);
      for (int i = 1; i < depth; i++) {
        array_type = llvm::ArrayType::get(array_type, (*array_sizes)[depth - i - 1]);
      }
      return array_type;
  } else if (kind == BASK_FUNCTION_TYPEKIND) {
      std::vector<llvm::Type *> params;
      for (auto it = signature->begin(); it != signature->end() - 1; ++it) {
        params.push_back(it->get_llvm_type(TheContext));
      }
      return llvm::FunctionType::get(signature->back().get_llvm_type(TheContext), params, false);
  }
  return nullptr;
}