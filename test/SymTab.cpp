#include <gtest/gtest.h>
#include "symtab.h"
#include <iostream>

using namespace std;

TEST(SymTab, AddScope) {
  SymTab symtab;
  cout << symtab.get_scope_depth();
  
  symtab.enter_block();

  EXPECT_EQ(symtab.get_scope_depth(), 2);
}

TEST(SymTab, AddNameType1Scope) {
  SymTab symtab;
  symtab.add_name("a", KL_Type(false, KL_INT));
  symtab.add_name("b", KL_Type(false, KL_FLOAT));
  symtab.add_name("c", KL_Type(false, KL_BOOL));
  symtab.add_name("d", KL_Type(false, KL_CHAR));
  symtab.add_name("e", KL_Type(false, KL_STRING));

  EXPECT_EQ(symtab.get_name_type("a"), KL_Type(false, KL_INT));
  EXPECT_EQ(symtab.get_name_type("b"), KL_Type(false, KL_FLOAT));
  EXPECT_EQ(symtab.get_name_type("c"), KL_Type(false, KL_BOOL));
  EXPECT_EQ(symtab.get_name_type("d"), KL_Type(false, KL_CHAR));
  EXPECT_EQ(symtab.get_name_type("e"), KL_Type(false, KL_STRING));
}

TEST(SymTab, AddNameType2Scopes) {
  SymTab symtab;
  symtab.add_name("a", KL_Type(false, KL_INT));
  symtab.add_name("b", KL_Type(false, KL_FLOAT));

  symtab.enter_block();
  symtab.print();

  symtab.add_name("c", KL_Type(false, KL_BOOL));
  symtab.add_name("d", KL_Type(false, KL_CHAR));
  symtab.add_name("e", KL_Type(false, KL_STRING));

  EXPECT_EQ(symtab.get_name_type("a"), KL_Type(false, KL_INT));
  EXPECT_EQ(symtab.get_name_type("b"), KL_Type(false, KL_FLOAT));
  EXPECT_EQ(symtab.get_name_type("c"), KL_Type(false, KL_BOOL));
  EXPECT_EQ(symtab.get_name_type("d"), KL_Type(false, KL_CHAR));
  EXPECT_EQ(symtab.get_name_type("e"), KL_Type(false, KL_STRING));

  symtab.print();
  symtab.exit_block();

  EXPECT_EQ(symtab.get_name_type("a"), KL_Type(false, KL_INT));
  EXPECT_EQ(symtab.get_name_type("b"), KL_Type(false, KL_FLOAT));
  EXPECT_EQ(symtab.get_name_type("c"), nullopt);
  EXPECT_EQ(symtab.get_name_type("d"), nullopt);
  EXPECT_EQ(symtab.get_name_type("e"), nullopt);
}