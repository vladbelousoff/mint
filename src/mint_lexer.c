#include "mint_lexer.h"

#include "mint_token.h"
#include "rtl_memory.h"

#include <stdlib.h>
#include <string.h>

#include "rtl_log.h"

void mint_lexer_init(struct mint_lexer* self)
{
  rtl_list_init(&self->tokens);
  self->position = 0;
  self->line = 1;
  self->column = 1;
}

void mint_lexer_cleanup(struct mint_lexer* self)
{
  struct rtl_list_entry* position;
  struct rtl_list_entry* n;

  rtl_list_for_each_safe(position, n, &self->tokens)
  {
    rtl_list_remove(position);
    rtl_free(rtl_list_record(position, struct mint_token, link));
  }

  self->position = 0;
  self->line = 0;
  self->column = 0;
}

static void mint_add_token(
  struct mint_lexer* lexer, enum mint_token_id id, const char* buffer, unsigned int buffer_size)
{
  unsigned int token_size = sizeof(struct mint_token);
  if (buffer_size > 0) {
    token_size += buffer_size + 1;
  }

  struct mint_token* token = rtl_malloc(token_size);
  token->id = id;
  token->line = lexer->line;
  token->column = lexer->column;
  token->position = lexer->position;
  token->buffer_size = buffer_size;

  if (buffer) {
    memcpy(token->buffer, buffer, buffer_size);
  }

  if (buffer_size > 0) {
    token->buffer[buffer_size] = 0;
  }

  /* Adding token to the list */
  rtl_list_add_tail(&lexer->tokens, &token->link);
}

static bool mint_lexer_try_scan_comments(struct mint_lexer* lexer, const char* input)
{
  if (input[lexer->position] == '/') {
    unsigned long position = lexer->position + 2;
    switch (input[lexer->position + 1]) {
      case '*':
        while (true) {
          if (input[position] == 0) {
            // Invalid comment section
            return false;
          }
          if (input[position] == '\n') {
            position++;
            lexer->line++;
            lexer->column = 1;
          } else if (input[position] == '*' && input[position + 1] == '/') {
            // The section is correct, just return true
            lexer->position = position + 2;
            lexer->column += 2;
            return true;
          } else {
            position++;
            lexer->column++;
          }
        }
      case '/':
        while (true) {
          if (input[position] == 0) {
            // Invalid comment section
            return false;
          }
          if (input[position] == '\n') {
            lexer->line += 1;
            lexer->column = 1;
            lexer->position = position + 1;
            return true;
          }
          position++;
          lexer->column++;
        }
      default:
        return false;
    }
  }

  return false;
}

static bool mint_lexer_try_scan_operator(struct mint_lexer* lexer, const char* input)
{
  switch (input[lexer->position]) {
    case '+':
      if (input[lexer->position + 1] == '=') {
        mint_add_token(lexer, MINT_TOKEN_ID_PLUS_ASSIGN, NULL, 0);
        lexer->position += 2;
      } else {
        mint_add_token(lexer, MINT_TOKEN_ID_PLUS, NULL, 0);
        lexer->position += 1;
      }
      return true;
    case '-':
      if (input[lexer->position + 1] == '=') {
        mint_add_token(lexer, MINT_TOKEN_ID_MINUS_ASSIGN, NULL, 0);
        lexer->position += 2;
      } else {
        mint_add_token(lexer, MINT_TOKEN_ID_MINUS, NULL, 0);
        lexer->position += 1;
      }
      return true;
    case '*':
      if (input[lexer->position + 1] == '=') {
        mint_add_token(lexer, MINT_TOKEN_ID_TIMES_ASSIGN, NULL, 0);
        lexer->position += 2;
      } else {
        mint_add_token(lexer, MINT_TOKEN_ID_TIMES, NULL, 0);
        lexer->position += 1;
      }
      return true;
    case '/':
      if (input[lexer->position + 1] == '=') {
        mint_add_token(lexer, MINT_TOKEN_ID_DIVIDE_ASSIGN, NULL, 0);
        lexer->position += 2;
      } else {
        mint_add_token(lexer, MINT_TOKEN_ID_DIVIDE, NULL, 0);
        lexer->position += 1;
      }
      return true;
    case '=':
      if (input[lexer->position + 1] == '=') {
        mint_add_token(lexer, MINT_TOKEN_ID_EQUAL, NULL, 0);
        lexer->position += 2;
      } else {
        mint_add_token(lexer, MINT_TOKEN_ID_ASSIGN, NULL, 0);
        lexer->position += 1;
      }
      return true;
    default:
      return false;
  }
}

static void mint_lexer_skip_whitespaces(struct mint_lexer* lexer, const char* input)
{
  while (true) {
    const char c = input[lexer->position];
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        lexer->column++;
        lexer->position++;
        break;
      case '\n':
        lexer->column = 1;
        lexer->position++;
        lexer->line++;
        break;
      default:
        return;
    }
  }
}

static bool is_digit(char c)
{
  return c >= '0' && c <= '9';
}

static bool mint_lexer_try_scan_number(struct mint_lexer* lexer, const char* input)
{
  const char first_char = input[lexer->position];

  // Check if the current character is a digit
  if (is_digit(first_char)) {
    unsigned int start_position = lexer->position;
    unsigned int current_position = start_position;
    bool is_float = false;

    // Scan the number
    while (true) {
      char c = input[current_position];

      // Break if end of stream
      if (c == 0) {
        break;
      }

      // Check for digits
      if (c >= '0' && c <= '9') {
        current_position++;
        continue;
      }

      // Check for decimal point (only allowed once)
      if (c == '.' && !is_float) {
        // Ensure the next character is a digit
        if (is_digit(input[current_position + 1])) {
          is_float = true;
          current_position++;
          continue;
        }
      }

      // If we reach here, we've hit the end of the number
      break;
    }

    // Calculate the length of the number
    unsigned int length = current_position - start_position;

    // Create a buffer for the number string
    const char* buffer = &input[start_position];

    // Add the number token
    mint_add_token(lexer, MINT_TOKEN_ID_NUMBER, buffer, length);

    // Update the lexer position and column
    lexer->column += length;
    lexer->position = current_position;

    return true;
  }

  return false;
}

static bool is_letter(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_identifier_char(char c)
{
  return is_letter(c) || is_digit(c) || c == '_';
}

static bool mint_lexer_try_scan_identifier(struct mint_lexer* lexer, const char* input)
{
  if (is_letter(input[lexer->position]) || input[lexer->position] == '_') {
    unsigned int start_position = lexer->position;
    unsigned int current_position = start_position;

    // Scan the identifier
    while (is_identifier_char(input[current_position])) {
      current_position++;
    }

    // Calculate the length of the identifier
    unsigned int length = current_position - start_position;

    // Check if it's a keyword
    const char* buffer = &input[start_position];
    enum mint_token_id token_id = MINT_TOKEN_ID_IDENTIFIER;

    // Compare with keywords
    if (length == 6 && strncmp(buffer, "struct", 6) == 0) {
      token_id = MINT_TOKEN_ID_STRUCT;
    } else if (length == 5 && strncmp(buffer, "trait", 5) == 0) {
      token_id = MINT_TOKEN_ID_TRAIT;
    } else if (length == 4 && strncmp(buffer, "impl", 4) == 0) {
      token_id = MINT_TOKEN_ID_IMPL;
    } else if (length == 3 && strncmp(buffer, "for", 3) == 0) {
      token_id = MINT_TOKEN_ID_FOR;
    } else if (length == 2 && strncmp(buffer, "fn", 2) == 0) {
      token_id = MINT_TOKEN_ID_FN;
    } else if (length == 6 && strncmp(buffer, "return", 6) == 0) {
      token_id = MINT_TOKEN_ID_RETURN;
    } else if (length == 4 && strncmp(buffer, "self", 4) == 0) {
      token_id = MINT_TOKEN_ID_SELF;
    } else if (length == 3 && strncmp(buffer, "let", 3) == 0) {
      token_id = MINT_TOKEN_ID_LET;
    } else if (length == 3 && strncmp(buffer, "mut", 3) == 0) {
      token_id = MINT_TOKEN_ID_MUT;
    } else if (length == 2 && strncmp(buffer, "if", 2) == 0) {
      token_id = MINT_TOKEN_ID_IF;
    } else if (length == 4 && strncmp(buffer, "else", 4) == 0) {
      token_id = MINT_TOKEN_ID_ELSE;
    } else if (length == 5 && strncmp(buffer, "while", 5) == 0) {
      token_id = MINT_TOKEN_ID_WHILE;
    } else if (length == 4 && strncmp(buffer, "true", 4) == 0) {
      token_id = MINT_TOKEN_ID_TRUE;
    } else if (length == 5 && strncmp(buffer, "false", 5) == 0) {
      token_id = MINT_TOKEN_ID_FALSE;
    }

    // Add the token
    mint_add_token(lexer, token_id, buffer, length);

    // Update the lexer position and column
    lexer->column += length;
    lexer->position = current_position;

    return true;
  }

  return false;
}

static void mint_report_unknown_character(const struct mint_lexer* lexer, const char* input)
{
  const char c = input[lexer->position];
  if (c != 0) {
    rtl_log_e("Unknown character: '%c', line: %u, column: %u, position: %u", c, lexer->line,
      lexer->column, lexer->position);
    exit(-1);
  }
}

void mint_lexer_tokenize(struct mint_lexer* lexer, const char* input)
{
  while (input[lexer->position]) {
    mint_lexer_skip_whitespaces(lexer, input);

    if (mint_lexer_try_scan_comments(lexer, input)) {
      continue;
    }

    if (mint_lexer_try_scan_operator(lexer, input)) {
      continue;
    }

    if (mint_lexer_try_scan_number(lexer, input)) {
      continue;
    }

    if (mint_lexer_try_scan_identifier(lexer, input)) {
      continue;
    }

    mint_report_unknown_character(lexer, input);
  }
}
