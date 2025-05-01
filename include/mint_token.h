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

enum mint_token_id {
  MINT_TOKEN_ID_UNKNOWN = 0,
  MINT_TOKEN_ID_DEF = 1,
  MINT_TOKEN_ID_LBRACE = 2,
  MINT_TOKEN_ID_RBRACE = 3,
  MINT_TOKEN_ID_LPAREN = 4,
  MINT_TOKEN_ID_RPAREN = 5,
  MINT_TOKEN_ID_LBRACK = 6,
  MINT_TOKEN_ID_RBRACK = 7,
  MINT_TOKEN_ID_IDENTIFIER = 8,
  MINT_TOKEN_ID_NUMBER = 9,
  MINT_TOKEN_ID_STRING = 10,
  MINT_TOKEN_ID_PLUS = 11,
  MINT_TOKEN_ID_MINUS = 12,
  MINT_TOKEN_ID_TIMES = 13,
  MINT_TOKEN_ID_DIVIDE = 14,
  MINT_TOKEN_ID_MOD = 15,
  MINT_TOKEN_ID_GREATER = 16,
  MINT_TOKEN_ID_LESS = 17,
  MINT_TOKEN_ID_EQUAL = 18,
  MINT_TOKEN_ID_NOTEQUAL = 19,
  MINT_TOKEN_ID_ASSIGN = 20,
};

struct mint_token
{
  struct rtl_list_entry link;
  enum mint_token_id id;
  int line;
  int column;
  int position;
  int buffer_size;
  char buffer[0];
};

const char* mint_token_id_string(enum mint_token_id id);
