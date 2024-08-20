#include <gtest/gtest.h>
#include "types.h"

#define type_to_string(type) type.to_string()

TEST(Types, TypeToStringPrimitive) {
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_INT_PRIMITIVE)), "int");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_FLOAT_PRIMITIVE)), "float");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_BOOL_PRIMITIVE)), "bool");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_CHAR_PRIMITIVE)), "char");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_STRING_PRIMITIVE)), "string");

  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_INT_PRIMITIVE)), "const int");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_FLOAT_PRIMITIVE)), "const float");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_BOOL_PRIMITIVE)), "const bool");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_CHAR_PRIMITIVE)), "const char");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_STRING_PRIMITIVE)), "const string");
}

TEST(Types, TypesToStringArray) {
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_INT_PRIMITIVE, new vector<unsigned int>{3})), "int[3]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_INT_PRIMITIVE, new vector<unsigned int>{3, 4})), "int[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_INT_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "int[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_INT_PRIMITIVE, new vector<unsigned int>{3})), "const int[3]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_INT_PRIMITIVE, new vector<unsigned int>{3, 4})), "const int[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_INT_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "const int[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_FLOAT_PRIMITIVE, new vector<unsigned int>{3})), "float[3]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_FLOAT_PRIMITIVE, new vector<unsigned int>{3, 4})), "float[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_FLOAT_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "float[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_FLOAT_PRIMITIVE, new vector<unsigned int>{3})), "const float[3]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_FLOAT_PRIMITIVE, new vector<unsigned int>{3, 4})), "const float[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_FLOAT_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "const float[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_BOOL_PRIMITIVE, new vector<unsigned int>{3})), "bool[3]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_BOOL_PRIMITIVE, new vector<unsigned int>{3, 4})), "bool[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_BOOL_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "bool[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_BOOL_PRIMITIVE, new vector<unsigned int>{3})), "const bool[3]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_BOOL_PRIMITIVE, new vector<unsigned int>{3, 4})), "const bool[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_BOOL_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "const bool[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_CHAR_PRIMITIVE, new vector<unsigned int>{3})), "char[3]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_CHAR_PRIMITIVE, new vector<unsigned int>{3, 4})), "char[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_CHAR_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "char[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_CHAR_PRIMITIVE, new vector<unsigned int>{3})), "const char[3]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_CHAR_PRIMITIVE, new vector<unsigned int>{3, 4})), "const char[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(true, BASK_CHAR_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "const char[3, 4, 5]");

  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_STRING_PRIMITIVE, new vector<unsigned int>{3})), "string[3]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_STRING_PRIMITIVE, new vector<unsigned int>{3, 4})), "string[3, 4]");
  ASSERT_EQ(type_to_string(BASK_Type(false, BASK_STRING_PRIMITIVE, new vector<unsigned int>{3, 4, 5})), "string[3, 4, 5]");
}

TEST(Types, TypesToStringFunction) {
  BASK_Type argc = BASK_Type{false, BASK_INT_PRIMITIVE};
  BASK_Type argv = BASK_Type{false, BASK_STRING_PRIMITIVE, new vector<unsigned int>{3}};

  BASK_Type c_main_func_signature = BASK_Type(false, new vector<BASK_Type>{argc, argv, BASK_Type(false, BASK_INT_PRIMITIVE)});

  ASSERT_EQ(c_main_func_signature.to_string(), "(int -> string[3] -> int)");
}