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
  Token expected[] = {Token{KLTokenType::KLTT_KW_If, "if", 1, 1},
                      Token{KLTokenType::KLTT_KW_Else, "else", 1, 4},
                      Token{KLTokenType::KLTT_KW_While, "while", 2, 2},
                      Token{KLTokenType::KLTT_KW_For, "for", 2, 8},
                      Token{KLTokenType::KLTT_KW_Return, "return", 2, 12},
                      Token{KLTokenType::KLTT_KW_Const, "const", 3, 2},
                      Token{KLTokenType::KLTT_KW_Int, "int", 3, 8},
                      Token{KLTokenType::KLTT_KW_Float, "float", 3, 12},
                      Token{KLTokenType::KLTT_KW_Char, "char", 3, 18},
                      Token{KLTokenType::KLTT_KW_Bool, "bool", 3, 23}};
  
  Options options(2, argv);
  Lexer lexer(options);

  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << tokenToString(i);
    }
    tokenToString((const Token &) token);
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, Punctuation) {
  const char* argv[] = {"KL", insertIntoTempFile("(){}[];.,= == != < <= > >= << >>")};
  Token expected[] = {Token{KLTokenType::KLTT_Punctuation_LParen, "(", 1, 1},
                      Token{KLTokenType::KLTT_Punctuation_RParen, ")", 1, 2},
                      Token{KLTokenType::KLTT_Punctuation_LBrace, "{", 1, 3},
                      Token{KLTokenType::KLTT_Punctuation_RBrace, "}", 1, 4},
                      Token{KLTokenType::KLTT_Punctuation_LBracket, "[", 1, 5},
                      Token{KLTokenType::KLTT_Punctuation_RBracket, "]", 1, 6},
                      Token{KLTokenType::KLTT_Punctuation_Semicolon, ";", 1, 7},
                      Token{KLTokenType::KLTT_Punctuation_Dot, ".", 1, 8},
                      Token{KLTokenType::KLTT_Punctuation_Comma, ",", 1, 9},
                      Token{KLTokenType::KLTT_Operator_Assign, "=", 1, 10},
                      Token{KLTokenType::KLTT_Operator_Equal, "==", 1, 12},
                      Token{KLTokenType::KLTT_Operator_NotEqual, "!=", 1, 15},
                      Token{KLTokenType::KLTT_Operator_Less, "<", 1, 18},
                      Token{KLTokenType::KLTT_Operator_LessEqual, "<=", 1, 20},
                      Token{KLTokenType::KLTT_Operator_Greater, ">", 1, 23},
                      Token{KLTokenType::KLTT_Operator_GreaterEqual, ">=", 1, 25},
                      Token{KLTokenType::KLTT_Operator_Shl, "<<", 1, 28},
                      Token{KLTokenType::KLTT_Operator_Shr, ">>", 1, 31}};
  
  Options options(2, argv);
  Lexer lexer(options);

  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << tokenToString(i);
    }
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, Operators) {
  const char* argv[] = {"KL", insertIntoTempFile("+-*/%&|!^=&&||")};
  Token expected[] = {Token{KLTokenType::KLTT_Operator_Add, "+", 1, 1},
                      Token{KLTokenType::KLTT_Operator_Sub, "-", 1, 2},
                      Token{KLTokenType::KLTT_Operator_Mul, "*", 1, 3},
                      Token{KLTokenType::KLTT_Operator_Div, "/", 1, 4},
                      Token{KLTokenType::KLTT_Operator_Mod, "%", 1, 5},
                      Token{KLTokenType::KLTT_Operator_BitwiseAnd, "&", 1, 6},
                      Token{KLTokenType::KLTT_Operator_BitwiseOr, "|", 1, 7},
                      Token{KLTokenType::KLTT_Operator_LogicalNot, "!", 1, 8},
                      Token{KLTokenType::KLTT_Operator_BitwiseXor, "^", 1, 9},
                      Token{KLTokenType::KLTT_Operator_Assign, "=", 1, 10},
                      Token{KLTokenType::KLTT_Operator_LogicalAnd, "&&", 1, 11},
                      Token{KLTokenType::KLTT_Operator_LogicalOr, "||", 1, 13}};
  
  Options options(2, argv);
  Lexer lexer(options);

  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << tokenToString(i);
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
  
  Token expected[] = {Token{KLTokenType::KLTT_Identifier, "a", 1, 1},
                      Token{KLTokenType::KLTT_Identifier, "b", 1, 3},
                      Token{KLTokenType::KLTT_Identifier, "c", 1, 5},
                      Token{KLTokenType::KLTT_Identifier, "A", 2, 1},
                      Token{KLTokenType::KLTT_Identifier, "B", 2, 3},
                      Token{KLTokenType::KLTT_Identifier, "C", 2, 5},
                      Token{KLTokenType::KLTT_Identifier, "_", 3, 1},
                      Token{KLTokenType::KLTT_Identifier, "__", 3, 3},
                      Token{KLTokenType::KLTT_Identifier, "___", 3, 6},
                      Token{KLTokenType::KLTT_Identifier, "_a", 3, 10},
                      Token{KLTokenType::KLTT_Identifier, "_1", 3, 13},
                      Token{KLTokenType::KLTT_Identifier, "_a1", 3, 16},
                      Token{KLTokenType::KLTT_Identifier, "_1a", 3, 20},
                      Token{KLTokenType::KLTT_Identifier, "longer_identifier", 4, 1}};
  
  Options options(2, argv);
  Lexer lexer(options);
  
  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << tokenToString(i);
    }
    ASSERT_EQ(token.value().type, i.type);
    ASSERT_EQ(token.value().value, i.value);
    ASSERT_EQ(token.value().line, i.line);
    ASSERT_EQ(token.value().col, i.col);
  }
}

TEST(Lexer, CharLiterals) {
  const char *argv[] = {"KL", insertIntoTempFile("'a' 'b' 'c' '\\n' '\\t' '\\0' '\\'' '\\\\'")};
  
  Token expected[] = {Token{KLTokenType::KLTT_Literal_Char, "a", 1, 1},
                      Token{KLTokenType::KLTT_Literal_Char, "b", 1, 5},
                      Token{KLTokenType::KLTT_Literal_Char, "c", 1, 9},
                      Token{KLTokenType::KLTT_Literal_Char, "\n", 1, 13},
                      Token{KLTokenType::KLTT_Literal_Char, "\t", 1, 18},
                      Token{KLTokenType::KLTT_Literal_Char, "\0", 1, 23},
                      Token{KLTokenType::KLTT_Literal_Char, "'", 1, 28},
                      Token{KLTokenType::KLTT_Literal_Char, "\\", 1, 33}};
  
  Options options(2, argv);
  Lexer lexer(options);
  
  for (auto & i : expected) {
    auto token = lexer.next();
    if (!token.has_value()) {
      FAIL() << string("Expected token not found. Expected: ") << tokenToString(i);
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