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

static bool mint_lexer_try_scan_number(struct mint_lexer* lexer, const char* input)
{
  const char first_char = input[lexer->position];

  // Check if the current character is a digit
  if (first_char >= '0' && first_char <= '9') {
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
        if (input[current_position + 1] >= '0' && input[current_position + 1] <= '9') {
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

    mint_report_unknown_character(lexer, input);
  }
}
