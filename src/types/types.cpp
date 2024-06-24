//
// Created by kiran on 5/21/24.
//

#include "types.h"

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