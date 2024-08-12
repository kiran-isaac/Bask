//
// Created by kiran on 5/20/24.
//

#ifndef KL_TYPES_H
#define KL_TYPES_H

#include <algorithm>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

enum KL_PrimitiveType {
  KL_INT,
  KL_FLOAT,
  KL_BOOL,
  KL_CHAR,
  KL_STRING,
  KL_VOID
};

enum KL_TypeKind { KL_PRIMITIVE, KL_ARRAY, KL_FUNCTION };

struct KL_Type {
  bool is_const;
  KL_TypeKind kind;
  KL_PrimitiveType primitive;
  union {
    vector<unsigned int> *array_sizes;
    vector<KL_Type> *signature;
  };

  // Default constructor is non const int. Only used for parsing
  KL_Type() : is_const(false), kind(KL_PRIMITIVE), primitive(KL_INT) {}

  // Copy constructor
  KL_Type(const KL_Type &other) {
    is_const = other.is_const;
    kind = other.kind;
    primitive = other.primitive;
    if (kind == KL_ARRAY) {
      array_sizes = new vector<unsigned int>(*other.array_sizes);
    } else if (kind == KL_FUNCTION) {
      signature = new vector<KL_Type>(*other.signature);
    }
  }

  // Primitive type
  KL_Type(bool is_const, KL_PrimitiveType primitive)
      : is_const(is_const), kind(KL_PRIMITIVE), primitive(primitive) {}

  // Array type
  KL_Type(bool is_const, KL_PrimitiveType primitive,
          vector<unsigned int> *array_sizes)
      : is_const(is_const), kind(KL_ARRAY), primitive(primitive),
        array_sizes(array_sizes) {}

  // Array type
  KL_Type(bool is_const, KL_PrimitiveType primitive,
          vector<unsigned int> array_sizes)
      : is_const(is_const), kind(KL_ARRAY), primitive(primitive) {
    this->array_sizes = new vector<unsigned int>(array_sizes);
  }

  // Function type
  KL_Type(bool is_const, vector<KL_Type> *signature)
      : is_const(is_const), kind(KL_FUNCTION), primitive(KL_VOID),
        signature(signature) {}

  ~KL_Type() {
    if (kind == KL_ARRAY) {
      delete array_sizes;
    } else if (kind == KL_FUNCTION) {
      delete signature;
    }
  };

  // To string
  string to_string() const;

  bool operator==(const KL_Type &other) const {
    if (is_const != other.is_const)
      return false;
    if (kind != other.kind)
      return false;
    if (primitive != other.primitive)
      return false;
    if (kind == KL_ARRAY) {
      if (*array_sizes != *other.array_sizes)
        return false;
    } else if (kind == KL_FUNCTION) {
      if (*signature != *other.signature)
        return false;
    }
    return true;
  }

  bool operator!=(const KL_Type &other) const { return !(*this == other); }

  KL_Type get_return_type() {
    if (kind != KL_FUNCTION) {
      throw std::runtime_error("Type is not a function");
    }
    return signature->back();
  }

  llvm::Type *get_llvm_type(llvm::LLVMContext &TheContext) const;
};

string primitive_to_string(const KL_PrimitiveType &type);

#define BOOL_CONST KL_Type(true, KL_BOOL)
#define INT_CONST KL_Type(true, KL_INT)
#define FLOAT_CONST KL_Type(true, KL_FLOAT)
#define CHAR_CONST KL_Type(true, KL_CHAR)
#define STRING_CONST KL_Type(true, KL_STRING)

#endif // KL_TYPES_H
