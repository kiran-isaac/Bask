//
// Created by kiran on 5/20/24.
//

#ifndef KL_TYPES_H
#define KL_TYPES_H

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


#endif //KL_TYPES_H
