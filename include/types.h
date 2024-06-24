//
// Created by kiran on 5/20/24.
//

#ifndef KL_TYPES_H
#define KL_TYPES_H

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

enum KL_TypeKind {
  KL_PRIMITIVE,
  KL_ARRAY,
  KL_FUNCTION
};

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

  // Primitive type
  KL_Type(bool is_const, KL_PrimitiveType primitive) : is_const(is_const), kind(KL_PRIMITIVE), primitive(primitive) {}

  // Array type
  KL_Type(bool is_const, KL_PrimitiveType primitive, vector<unsigned int> *array_sizes) : is_const(is_const), kind(KL_ARRAY), primitive(primitive), array_sizes(array_sizes) {}
  
  // Function type
  KL_Type(bool is_const, vector<KL_Type> *signature) : is_const(is_const), kind(KL_FUNCTION), primitive(primitive), signature(signature) {}

  ~KL_Type() {
    if (kind == KL_ARRAY) {
      delete array_sizes;
    } else if (kind == KL_FUNCTION) {
      delete signature;
    }
  };

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

  // To string
  string to_string() const;

  bool operator==(const KL_Type &other) const {
    if (is_const != other.is_const) return false;
    if (kind != other.kind) return false;
    if (primitive != other.primitive) return false;
    if (kind == KL_ARRAY) {
      if (*array_sizes != *other.array_sizes) return false;
    } else if (kind == KL_FUNCTION) {
      if (*signature != *other.signature) return false;
    }
    return true;
  }
};

#define BOOL_CONST KL_Type(true, KL_BOOL)
#define INT_CONST KL_Type(true, KL_INT)
#define FLOAT_CONST KL_Type(true, KL_FLOAT)
#define CHAR_CONST KL_Type(true, KL_CHAR)
#define STRING_CONST KL_Type(true, KL_STRING)

#endif //KL_TYPES_H
