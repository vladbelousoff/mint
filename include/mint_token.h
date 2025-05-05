// MIT License
//
// Copyright (c) 2025 Vladislav Belousov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <rtl_list.h>

enum mint_token_id
{
  MINT_TOKEN_ID_UNKNOWN = 0,

  /* Symbols */
  MINT_TOKEN_ID_LBRACE = 1,
  MINT_TOKEN_ID_RBRACE = 2,
  MINT_TOKEN_ID_LPAREN = 3,
  MINT_TOKEN_ID_RPAREN = 4,
  MINT_TOKEN_ID_LBRACK = 5,
  MINT_TOKEN_ID_RBRACK = 6,
  MINT_TOKEN_ID_COLON = 7,
  MINT_TOKEN_ID_SEMICOLON = 8,
  MINT_TOKEN_ID_ARROW = 9,

  /* Literals */
  MINT_TOKEN_ID_IDENTIFIER = 10,
  MINT_TOKEN_ID_NUMBER = 11,
  MINT_TOKEN_ID_FLOAT = 12,
  MINT_TOKEN_ID_STRING = 13,

  /* Operators */
  MINT_TOKEN_ID_PLUS = 14,
  MINT_TOKEN_ID_MINUS = 15,
  MINT_TOKEN_ID_TIMES = 16,
  MINT_TOKEN_ID_DIVIDE = 17,
  MINT_TOKEN_ID_PLUS_ASSIGN = 18,
  MINT_TOKEN_ID_MINUS_ASSIGN = 19,
  MINT_TOKEN_ID_TIMES_ASSIGN = 20,
  MINT_TOKEN_ID_DIVIDE_ASSIGN = 21,
  MINT_TOKEN_ID_GREATER = 22,
  MINT_TOKEN_ID_LESS = 23,
  MINT_TOKEN_ID_GREATER_EQUAL = 24,
  MINT_TOKEN_ID_LESS_EQUAL = 25,
  MINT_TOKEN_ID_EQUAL = 26,
  MINT_TOKEN_ID_NOTEQUAL = 27,
  MINT_TOKEN_ID_ASSIGN = 28,

  /* Keywords */
  MINT_TOKEN_ID_STRUCT = 29,
  MINT_TOKEN_ID_TRAIT = 30,
  MINT_TOKEN_ID_IMPL = 31,
  MINT_TOKEN_ID_FOR = 32,
  MINT_TOKEN_ID_FN = 33,
  MINT_TOKEN_ID_RETURN = 34,
  MINT_TOKEN_ID_SELF = 35,
  MINT_TOKEN_ID_LET = 36,
  MINT_TOKEN_ID_MUT = 37,
  MINT_TOKEN_ID_IF = 38,
  MINT_TOKEN_ID_ELSE = 39,
  MINT_TOKEN_ID_WHILE = 40,
  MINT_TOKEN_ID_TRUE = 41,
  MINT_TOKEN_ID_FALSE = 42,
};

struct mint_token
{
  struct rtl_list_entry link;
  enum mint_token_id id;
  unsigned int line;
  unsigned int column;
  unsigned int position;
  unsigned int buffer_size;
  char buffer[0];
};

const char* mint_token_id_string(enum mint_token_id id);
