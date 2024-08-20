//
// Created by kiran on 5/20/24.
//

#ifndef BASK_TYPES_H
#define BASK_TYPES_H

#include <algorithm>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum BASK_PrimitiveType {
  BASK_INT_PRIMITIVE,
  BASK_FLOAT_PRIMITIVE,
  BASK_BOOL_PRIMITIVE,
  BASK_CHAR_PRIMITIVE,
  BASK_STRING_PRIMITIVE,
  BASK_VOID_PRIMITIVE
};

enum BASK_TypeKind { BASK_PRIMITIVE_TYPEKIND, BASK_ARRAY_TYPEKIND, BASK_FUNCTION_TYPEKIND };

struct BASK_Type {
  bool is_const;
  BASK_TypeKind kind;
  BASK_PrimitiveType primitive;
  union {
    vector<unsigned int> *array_sizes;
    vector<BASK_Type> *signature;
  };

  // Default constructor is non const int. Only used for parsing
  BASK_Type() : is_const(false), kind(BASK_PRIMITIVE_TYPEKIND), primitive(BASK_INT_PRIMITIVE) {}

  // Copy constructor
  BASK_Type(const BASK_Type &other) {
    is_const = other.is_const;
    kind = other.kind;
    primitive = other.primitive;
    if (kind == BASK_ARRAY_TYPEKIND) {
      array_sizes = new vector<unsigned int>(*other.array_sizes);
    } else if (kind == BASK_FUNCTION_TYPEKIND) {
      signature = new vector<BASK_Type>(*other.signature);
    }
  }

  // Primitive type
  BASK_Type(bool is_const, BASK_PrimitiveType primitive)
      : is_const(is_const), kind(BASK_PRIMITIVE_TYPEKIND), primitive(primitive) {}

  // Array type
  BASK_Type(bool is_const, BASK_PrimitiveType primitive,
          vector<unsigned int> *array_sizes)
      : is_const(is_const), kind(BASK_ARRAY_TYPEKIND), primitive(primitive),
        array_sizes(array_sizes) {}

  // Array type
  BASK_Type(bool is_const, BASK_PrimitiveType primitive,
          vector<unsigned int> array_sizes)
      : is_const(is_const), kind(BASK_ARRAY_TYPEKIND), primitive(primitive) {
    this->array_sizes = new vector<unsigned int>(array_sizes);
  }

  // Function type
  BASK_Type(bool is_const, vector<BASK_Type> *signature)
      : is_const(is_const), kind(BASK_FUNCTION_TYPEKIND), primitive(BASK_VOID_PRIMITIVE),
        signature(signature) {}

  // Function type
  BASK_Type(bool is_const, vector<BASK_Type> signature)
      : is_const(is_const), kind(BASK_FUNCTION_TYPEKIND), primitive(BASK_VOID_PRIMITIVE) {
    this->signature = new vector<BASK_Type>(signature);
  }

  ~BASK_Type() {
    if (kind == BASK_ARRAY_TYPEKIND) {
      delete array_sizes;
    } else if (kind == BASK_FUNCTION_TYPEKIND) {
      delete signature;
    }
  };

  // To string
  string to_string() const;

  bool operator==(const BASK_Type &other) const {
    if (is_const != other.is_const)
      return false;
    if (kind != other.kind)
      return false;
    if (primitive != other.primitive)
      return false;
    if (kind == BASK_ARRAY_TYPEKIND) {
      if (*array_sizes != *other.array_sizes)
        return false;
    } else if (kind == BASK_FUNCTION_TYPEKIND) {
      if (*signature != *other.signature)
        return false;
    }
    return true;
  }

  bool operator!=(const BASK_Type &other) const { return !(*this == other); }

  BASK_Type get_return_type() {
    if (kind != BASK_FUNCTION_TYPEKIND) {
      throw std::runtime_error("Type is not a function");
    }
    return signature->back();
  }

  llvm::Type *get_llvm_type(llvm::LLVMContext &TheContext) const;
};

string primitive_to_string(const BASK_PrimitiveType &type);

#define BASK_BOOL_CONST BASK_Type(true, BASK_BOOL_PRIMITIVE)
#define BASK_INT_CONST BASK_Type(true, BASK_INT_PRIMITIVE)
#define BASK_FLOAT_CONST BASK_Type(true, BASK_FLOAT_PRIMITIVE)
#define BASK_CHAR_CONST BASK_Type(true, BASK_CHAR_PRIMITIVE)
#define BASK_STRING_CONST BASK_Type(true, BASK_STRING_PRIMITIVE)

#endif // BASK_TYPES_H
