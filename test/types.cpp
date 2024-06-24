#include <gtest/gtest.h>
#include "types.h"

#define type_to_string(type) type.to_string()

TEST(Types, TypeToStringPrimitive) {
  ASSERT_EQ(type_to_string(KL_Type(false, KL_INT)), "int");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_FLOAT)), "float");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_BOOL)), "bool");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_CHAR)), "char");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_STRING)), "string");

  ASSERT_EQ(type_to_string(KL_Type(true, KL_INT)), "const int");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_FLOAT)), "const float");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_BOOL)), "const bool");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_CHAR)), "const char");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_STRING)), "const string");
}

TEST(Types, TypesToStringArray) {
  ASSERT_EQ(type_to_string(KL_Type(false, KL_INT, new vector<unsigned int>{3})), "int[3]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_INT, new vector<unsigned int>{3, 4})), "int[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_INT, new vector<unsigned int>{3, 4, 5})), "int[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(true, KL_INT, new vector<unsigned int>{3})), "const int[3]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_INT, new vector<unsigned int>{3, 4})), "const int[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_INT, new vector<unsigned int>{3, 4, 5})), "const int[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(false, KL_FLOAT, new vector<unsigned int>{3})), "float[3]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_FLOAT, new vector<unsigned int>{3, 4})), "float[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_FLOAT, new vector<unsigned int>{3, 4, 5})), "float[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(true, KL_FLOAT, new vector<unsigned int>{3})), "const float[3]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_FLOAT, new vector<unsigned int>{3, 4})), "const float[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_FLOAT, new vector<unsigned int>{3, 4, 5})), "const float[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(false, KL_BOOL, new vector<unsigned int>{3})), "bool[3]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_BOOL, new vector<unsigned int>{3, 4})), "bool[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_BOOL, new vector<unsigned int>{3, 4, 5})), "bool[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(true, KL_BOOL, new vector<unsigned int>{3})), "const bool[3]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_BOOL, new vector<unsigned int>{3, 4})), "const bool[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_BOOL, new vector<unsigned int>{3, 4, 5})), "const bool[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(false, KL_CHAR, new vector<unsigned int>{3})), "char[3]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_CHAR, new vector<unsigned int>{3, 4})), "char[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_CHAR, new vector<unsigned int>{3, 4, 5})), "char[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(true, KL_CHAR, new vector<unsigned int>{3})), "const char[3]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_CHAR, new vector<unsigned int>{3, 4})), "const char[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(true, KL_CHAR, new vector<unsigned int>{3, 4, 5})), "const char[3, 4, 5]");

  ASSERT_EQ(type_to_string(KL_Type(false, KL_STRING, new vector<unsigned int>{3})), "string[3]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_STRING, new vector<unsigned int>{3, 4})), "string[3, 4]");
  ASSERT_EQ(type_to_string(KL_Type(false, KL_STRING, new vector<unsigned int>{3, 4, 5})), "string[3, 4, 5]");
}

TEST(Types, TypesToStringFunction) {
  KL_Type argc = KL_Type{false, KL_INT};
  KL_Type argv = KL_Type{false, KL_STRING, new vector<unsigned int>{3}};

  KL_Type c_main_func_signature = KL_Type(false, new vector<KL_Type>{argc, argv, KL_Type(false, KL_INT)});

  ASSERT_EQ(c_main_func_signature.to_string(), "(int -> string[3] -> int)");
}