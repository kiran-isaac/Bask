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
};

struct KL_Type {
  bool is_const;
  KL_TypeKind kind;
  KL_PrimitiveType primitive;
  vector<unsigned int> array_sizes;
};

#define BOOL_CONST KL_Type{true, KL_PRIMITIVE, KL_BOOL, {}}
#define INT_CONST KL_Type{true, KL_PRIMITIVE, KL_INT, {}}
#define FLOAT_CONST KL_Type{true, KL_PRIMITIVE, KL_FLOAT, {}}
#define CHAR_CONST KL_Type{true, KL_PRIMITIVE, KL_CHAR, {}}
#define STRING_CONST KL_Type{true, KL_PRIMITIVE, KL_STRING, {}}

string type_to_string(const KL_Type &type);

#endif //KL_TYPES_H
