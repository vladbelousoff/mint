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

#include "mint_token.h"

static char* mint_token_strings[] = {
  0,            /* UNKNOWN */
  
  /* Symbols */
  "{",          /* LBRACE */
  "}",          /* RBRACE */
  "(",          /* LPAREN */
  ")",          /* RPAREN */
  "[",          /* LBRACK */
  "]",          /* RBRACK */
  ":",          /* COLON */
  ";",          /* SEMICOLON */
  "->",         /* ARROW */
  
  /* Literals */
  0,            /* IDENTIFIER */
  0,            /* NUMBER */
  0,            /* FLOAT */
  0,            /* STRING */
  
  /* Operators */
  "+",          /* PLUS */
  "-",          /* MINUS */
  "*",          /* TIMES */
  "/",          /* DIVIDE */
  "+=",         /* PLUS_ASSIGN */
  "-=",         /* MINUS_ASSIGN */
  "*=",         /* TIMES_ASSIGN */
  "/=",         /* DIVIDE_ASSIGN */
  ">",          /* GREATER */
  "<",          /* LESS */
  ">=",         /* GREATER_EQUAL */
  "<=",         /* LESS_EQUAL */
  "==",         /* EQUAL */
  "!=",         /* NOTEQUAL */
  "=",          /* ASSIGN */
  
  /* Comments */
  0,            /* COMMENT_SINGLE */
  0,            /* COMMENT_MULTI */
  
  /* Keywords */
  "struct",     /* STRUCT */
  "trait",      /* TRAIT */
  "impl",       /* IMPL */
  "for",        /* FOR */
  "fn",         /* FN */
  "return",     /* RETURN */
  "self",       /* SELF */
  "let",        /* LET */
  "mut",        /* MUT */
  "if",         /* IF */
  "else",       /* ELSE */
  "while",      /* WHILE */
  "true",       /* TRUE */
  "false",      /* FALSE */
};

const char* mint_token_id_string(const enum mint_token_id id)
{
  return mint_token_strings[id];
}
