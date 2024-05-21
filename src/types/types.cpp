//
// Created by kiran on 5/21/24.
//

#include "types.h"

string type_to_string(const KL_Type &type)  {
  string str;
  if (type.is_const) {
    str += "const ";
  }
  switch (type.primitive) {
    case KL_INT:
      str += "int";
      break;
    case KL_FLOAT:
      str += "float";
      break;
    case KL_BOOL:
      str += "bool";
      break;
    case KL_CHAR:
      str += "char";
      break;
    case KL_STRING:
      str += "string";
      break;
    case KL_VOID:
      str += "void";
      break;
  }
  
  for (auto size : type.array_sizes) {
    str += "[" + to_string(size) + "]";
  }
  
  return str;
}