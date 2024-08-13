#include <gtest/gtest.h>
#include "symtab.h"
#include <iostream>

using namespace std;

TEST(SymTab, AddScope) {
  SymTab symtab;
  
  symtab.enter_block();

  EXPECT_EQ(symtab.get_scope_depth(), 2);
}

TEST(SymTab, AddNameType1Scope) {
  SymTab symtab;
  symtab.add_name("a", KL_Type(false, KL_INT_PRIMITIVE));
  symtab.add_name("b", KL_Type(false, KL_FLOAT_PRIMITIVE));
  symtab.add_name("c", KL_Type(false, KL_BOOL_PRIMITIVE));
  symtab.add_name("d", KL_Type(false, KL_CHAR_PRIMITIVE));
  symtab.add_name("e", KL_Type(false, KL_STRING_PRIMITIVE));

  EXPECT_EQ(symtab.get_name_type("a"), KL_Type(false, KL_INT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("b"), KL_Type(false, KL_FLOAT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("c"), KL_Type(false, KL_BOOL_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("d"), KL_Type(false, KL_CHAR_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("e"), KL_Type(false, KL_STRING_PRIMITIVE));
}

TEST(SymTab, AddNameType2Scopes) {
  SymTab symtab;
  symtab.add_name("a", KL_Type(false, KL_INT_PRIMITIVE));
  symtab.add_name("b", KL_Type(false, KL_FLOAT_PRIMITIVE));

  symtab.enter_block();

  symtab.add_name("c", KL_Type(false, KL_BOOL_PRIMITIVE));
  symtab.add_name("d", KL_Type(false, KL_CHAR_PRIMITIVE));
  symtab.add_name("e", KL_Type(false, KL_STRING_PRIMITIVE));

  EXPECT_EQ(symtab.get_name_type("a"), KL_Type(false, KL_INT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("b"), KL_Type(false, KL_FLOAT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("c"), KL_Type(false, KL_BOOL_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("d"), KL_Type(false, KL_CHAR_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("e"), KL_Type(false, KL_STRING_PRIMITIVE));

  symtab.exit_block();

  EXPECT_EQ(symtab.get_name_type("a"), KL_Type(false, KL_INT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("b"), KL_Type(false, KL_FLOAT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("c"), nullopt);
  EXPECT_EQ(symtab.get_name_type("d"), nullopt);
  EXPECT_EQ(symtab.get_name_type("e"), nullopt);
}
