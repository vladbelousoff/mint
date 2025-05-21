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
  MINT_TOKEN_ID_UNKNOWN,

  /* Symbols */
  MINT_TOKEN_ID_LBRACE,
  MINT_TOKEN_ID_RBRACE,
  MINT_TOKEN_ID_LPAREN,
  MINT_TOKEN_ID_RPAREN,
  MINT_TOKEN_ID_LBRACK,
  MINT_TOKEN_ID_RBRACK,
  MINT_TOKEN_ID_COLON,
  MINT_TOKEN_ID_SEMICOLON,
  MINT_TOKEN_ID_DOT,
  MINT_TOKEN_ID_COMMA,
  MINT_TOKEN_ID_ARROW,

  /* Literals */
  MINT_TOKEN_ID_IDENTIFIER,
  MINT_TOKEN_ID_NUMBER,
  MINT_TOKEN_ID_STRING,

  /* Operators */
  MINT_TOKEN_ID_PLUS,
  MINT_TOKEN_ID_MINUS,
  MINT_TOKEN_ID_TIMES,
  MINT_TOKEN_ID_DIVIDE,
  MINT_TOKEN_ID_PLUS_ASSIGN,
  MINT_TOKEN_ID_MINUS_ASSIGN,
  MINT_TOKEN_ID_TIMES_ASSIGN,
  MINT_TOKEN_ID_DIVIDE_ASSIGN,
  MINT_TOKEN_ID_GREATER,
  MINT_TOKEN_ID_LESS,
  MINT_TOKEN_ID_GREATER_EQUAL,
  MINT_TOKEN_ID_LESS_EQUAL,
  MINT_TOKEN_ID_EQUAL,
  MINT_TOKEN_ID_NOTEQUAL,
  MINT_TOKEN_ID_ASSIGN,

  /* Keywords */
  MINT_TOKEN_ID_STRUCT,
  MINT_TOKEN_ID_TRAIT,
  MINT_TOKEN_ID_IMPL,
  MINT_TOKEN_ID_FOR,
  MINT_TOKEN_ID_FN,
  MINT_TOKEN_ID_RETURN,
  MINT_TOKEN_ID_SELF,
  MINT_TOKEN_ID_LET,
  MINT_TOKEN_ID_MUT,
  MINT_TOKEN_ID_IF,
  MINT_TOKEN_ID_ELSE,
  MINT_TOKEN_ID_WHILE,
  MINT_TOKEN_ID_TRUE,
  MINT_TOKEN_ID_FALSE,
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
