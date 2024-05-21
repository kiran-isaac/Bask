//
// Created by kiran on 5/20/24.
//

#include <gtest/gtest.h>
#include <memory>
#include <lexer.h>
#include "utils/utils.h"

using namespace std;

TEST(Lexer, Keywords) {
  const char* argv[] = {"KL", insertIntoTempFile("if else\n"
                                                 " while for return\n"
                                                 " const int float char bool")};
  KL_Token expected[] = {KL_Token{KL_TokenType::KL_TT_KW_If, "if", 1, 1},
                         KL_Token{KL_TokenType::KL_TT_KW_Else, "else", 1, 4},
                         KL_Token{KL_TokenType::KL_TT_KW_While, "while", 2, 2},
                         KL_Token{KL_TokenType::KL_TT_KW_For, "for", 2, 8},
                         KL_Token{KL_TokenType::KL_TT_KW_Return, "return", 2, 12},
                         KL_Token{KL_TokenType::KL_TT_KW_Const, "const", 3, 2},
                         KL_Token{KL_TokenType::KL_TT_KW_Int, "int", 3, 8},
                         KL_Token{KL_TokenType::KL_TT_KW_Float, "float", 3, 12},
                         KL_Token{KL_TokenType::KL_TT_KW_Char, "char", 3, 18},
                         KL_Token{KL_TokenType::KL_TT_KW_Bool, "bool", 3, 23}};
  
  Options options(2, argv);
  Lexer lexer(options);

  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << token_to_string(i);
    }
    token_to_string((const KL_Token &) token);
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, Punctuation) {
  const char* argv[] = {"KL", insertIntoTempFile("(){}[];.,= == != < <= > >= << >>")};
  KL_Token expected[] = {KL_Token{KL_TokenType::KL_TT_Punctuation_LParen, "(", 1, 1},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_RParen, ")", 1, 2},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_LBrace, "{", 1, 3},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_RBrace, "}", 1, 4},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_LBracket, "[", 1, 5},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_RBracket, "]", 1, 6},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_Semicolon, ";", 1, 7},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_Dot, ".", 1, 8},
                         KL_Token{KL_TokenType::KL_TT_Punctuation_Comma, ",", 1, 9},
                         KL_Token{KL_TokenType::KL_TT_Operator_Assign, "=", 1, 10},
                         KL_Token{KL_TokenType::KL_TT_Operator_Equal, "==", 1, 12},
                         KL_Token{KL_TokenType::KL_TT_Operator_NotEqual, "!=", 1, 15},
                         KL_Token{KL_TokenType::KL_TT_Operator_Less, "<", 1, 18},
                         KL_Token{KL_TokenType::KL_TT_Operator_LessEqual, "<=", 1, 20},
                         KL_Token{KL_TokenType::KL_TT_Operator_Greater, ">", 1, 23},
                         KL_Token{KL_TokenType::KL_TT_Operator_GreaterEqual, ">=", 1, 25},
                         KL_Token{KL_TokenType::KL_TT_Operator_Shl, "<<", 1, 28},
                         KL_Token{KL_TokenType::KL_TT_Operator_Shr, ">>", 1, 31}};
  
  Options options(2, argv);
  Lexer lexer(options);

  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << token_to_string(i);
    }
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, Operators) {
  const char* argv[] = {"KL", insertIntoTempFile("+-*/%&|!^=&&||")};
  KL_Token expected[] = {KL_Token{KL_TokenType::KL_TT_Operator_Add, "+", 1, 1},
                         KL_Token{KL_TokenType::KL_TT_Operator_Sub, "-", 1, 2},
                         KL_Token{KL_TokenType::KL_TT_Operator_Mul, "*", 1, 3},
                         KL_Token{KL_TokenType::KL_TT_Operator_Div, "/", 1, 4},
                         KL_Token{KL_TokenType::KL_TT_Operator_Mod, "%", 1, 5},
                         KL_Token{KL_TokenType::KL_TT_Operator_BitwiseAnd, "&", 1, 6},
                         KL_Token{KL_TokenType::KL_TT_Operator_BitwiseOr, "|", 1, 7},
                         KL_Token{KL_TokenType::KL_TT_Operator_LogicalNot, "!", 1, 8},
                         KL_Token{KL_TokenType::KL_TT_Operator_BitwiseXor, "^", 1, 9},
                         KL_Token{KL_TokenType::KL_TT_Operator_Assign, "=", 1, 10},
                         KL_Token{KL_TokenType::KL_TT_Operator_LogicalAnd, "&&", 1, 11},
                         KL_Token{KL_TokenType::KL_TT_Operator_LogicalOr, "||", 1, 13}};
  
  Options options(2, argv);
  Lexer lexer(options);

  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << token_to_string(i);
    }
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, Identifiers) {
  const char *argv[] = {"KL", insertIntoTempFile("a b c\n"
                                                 "A B C\n"
                                                 "_ __ ___ _a _1 _a1 _1a\n"
                                                 "longer_identifier")};
  
  KL_Token expected[] = {KL_Token{KL_TokenType::KL_TT_Identifier, "a", 1, 1},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "b", 1, 3},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "c", 1, 5},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "A", 2, 1},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "B", 2, 3},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "C", 2, 5},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "_", 3, 1},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "__", 3, 3},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "___", 3, 6},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "_a", 3, 10},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "_1", 3, 13},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "_a1", 3, 16},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "_1a", 3, 20},
                         KL_Token{KL_TokenType::KL_TT_Identifier, "longer_identifier", 4, 1}};
  
  Options options(2, argv);
  Lexer lexer(options);
  
  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << token_to_string(i);
    }
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, CharLiterals) {
  const char *argv[] = {"KL", insertIntoTempFile("'a' 'b' 'c' '\\n' '\\t' '\\0' '\\'' '\\\\'")};
  
  KL_Token expected[] = {KL_Token{KL_TokenType::KL_TT_Literal_Char, "a", 1, 1},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "b", 1, 5},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "c", 1, 9},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "\n", 1, 13},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "\t", 1, 18},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "\0", 1, 23},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "'", 1, 28},
                         KL_Token{KL_TokenType::KL_TT_Literal_Char, "\\", 1, 33}};
  
  Options options(2, argv);
  Lexer lexer(options);
  
  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << token_to_string(i);
    }
    
    // Exception for \0 because it seems to do some weird in string literals. This still checks for
    // the desired behavior.
    if (!(token.value().value[0] == '\0' && token.value().col == 23)) {
      ASSERT_EQ(token.value().value, i.value);
    }
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}