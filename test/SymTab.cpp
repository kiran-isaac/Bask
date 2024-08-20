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
  symtab.add_name("a", BASK_Type(false, BASK_INT_PRIMITIVE));
  symtab.add_name("b", BASK_Type(false, BASK_FLOAT_PRIMITIVE));
  symtab.add_name("c", BASK_Type(false, BASK_BOOL_PRIMITIVE));
  symtab.add_name("d", BASK_Type(false, BASK_CHAR_PRIMITIVE));
  symtab.add_name("e", BASK_Type(false, BASK_STRING_PRIMITIVE));

  EXPECT_EQ(symtab.get_name_type("a"), BASK_Type(false, BASK_INT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("b"), BASK_Type(false, BASK_FLOAT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("c"), BASK_Type(false, BASK_BOOL_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("d"), BASK_Type(false, BASK_CHAR_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("e"), BASK_Type(false, BASK_STRING_PRIMITIVE));
}

TEST(SymTab, AddNameType2Scopes) {
  SymTab symtab;
  symtab.add_name("a", BASK_Type(false, BASK_INT_PRIMITIVE));
  symtab.add_name("b", BASK_Type(false, BASK_FLOAT_PRIMITIVE));

  symtab.enter_block();

  symtab.add_name("c", BASK_Type(false, BASK_BOOL_PRIMITIVE));
  symtab.add_name("d", BASK_Type(false, BASK_CHAR_PRIMITIVE));
  symtab.add_name("e", BASK_Type(false, BASK_STRING_PRIMITIVE));

  EXPECT_EQ(symtab.get_name_type("a"), BASK_Type(false, BASK_INT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("b"), BASK_Type(false, BASK_FLOAT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("c"), BASK_Type(false, BASK_BOOL_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("d"), BASK_Type(false, BASK_CHAR_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("e"), BASK_Type(false, BASK_STRING_PRIMITIVE));

  symtab.exit_block();

  EXPECT_EQ(symtab.get_name_type("a"), BASK_Type(false, BASK_INT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("b"), BASK_Type(false, BASK_FLOAT_PRIMITIVE));
  EXPECT_EQ(symtab.get_name_type("c"), nullopt);
  EXPECT_EQ(symtab.get_name_type("d"), nullopt);
  EXPECT_EQ(symtab.get_name_type("e"), nullopt);
}
