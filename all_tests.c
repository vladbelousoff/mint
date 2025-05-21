#include <stdio.h>
#include <stdlib.h>

#include "mint_lexer.h"
#include "mint_token.h"

#include <rtl_list.h>
#include <rtl_memory.h>
#include <rtl_test.h>

#include "rtl.h"

// Utility function to read a file content
static char* read_file_content(const char* file_path)
{
  FILE* file = fopen(file_path, "r");
  if (file == NULL) {
    rtl_log_e("Could not open test file: %s\n", file_path);
    return NULL;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  const long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate buffer and read file
  char* buffer = rtl_malloc(file_size + 1);
  if (buffer == NULL) {
    rtl_log_e("Memory allocation for file buffer failed\n");
    fclose(file);
    return NULL;
  }

  const size_t read_size = fread(buffer, 1, file_size, file);
  buffer[read_size] = 0;

  fclose(file);
  return buffer;
}

RTL_TEST_FUNCTION(test_001_empty)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "001_empty.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Verify we have some tokens
  int token_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    token_count++;
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  RTL_TEST_EQUAL(token_count, 0);

  return 0;
}

RTL_TEST_FUNCTION(test_002_comments)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "002_comments.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Verify we have some tokens
  int token_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    token_count++;
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  RTL_TEST_EQUAL(token_count, 0);

  return 0;
}

RTL_TEST_FUNCTION(test_003_numbers)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "003_numbers.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Verify we have the correct number of tokens
  int token_count = 0;
  int number_token_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    token_count++;

    if (token->id == MINT_TOKEN_ID_NUMBER) {
      number_token_count++;

      // Verify the token has the correct buffer content
      RTL_TEST_NOT_NULL(token->buffer);
      RTL_TEST_TRUE(token->buffer_size > 0);
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // We should have 8 number tokens: 4 integers and 4 floats
  RTL_TEST_EQUAL(token_count, 8);
  RTL_TEST_EQUAL(number_token_count, 8);

  return 0;
}

RTL_TEST_FUNCTION(test_004_identifiers)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "004_identifiers.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int total_tokens = 0;
  int identifier_count = 0;
  int keyword_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    total_tokens++;

    if (token->id == MINT_TOKEN_ID_IDENTIFIER) {
      identifier_count++;

      // Verify the token has a correct buffer
      RTL_TEST_NOT_NULL(token->buffer);
      RTL_TEST_TRUE(token->buffer_size > 0);
    } else if (token->id >= MINT_TOKEN_ID_STRUCT && token->id <= MINT_TOKEN_ID_FALSE) {
      keyword_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // We should have 5 identifiers and 14 keywords
  RTL_TEST_EQUAL(total_tokens, 19);
  RTL_TEST_EQUAL(identifier_count, 5);
  RTL_TEST_EQUAL(keyword_count, 14);

  return 0;
}

RTL_TEST_FUNCTION(test_005_symbols)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "005_symbols.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int total_tokens = 0;
  int symbol_count = 0;
  int operator_count = 0;
  int string_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    total_tokens++;

    if (token->id >= MINT_TOKEN_ID_LBRACE && token->id <= MINT_TOKEN_ID_ARROW) {
      symbol_count++;
    } else if (token->id >= MINT_TOKEN_ID_PLUS && token->id <= MINT_TOKEN_ID_ASSIGN) {
      operator_count++;
    } else if (token->id == MINT_TOKEN_ID_STRING) {
      string_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  RTL_TEST_TRUE(total_tokens > 0);
  RTL_TEST_TRUE(symbol_count > 0);
  RTL_TEST_TRUE(operator_count > 0);
  RTL_TEST_TRUE(string_count > 0);

  return 0;
}

RTL_TEST_FUNCTION(test_006_dot_comma)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "006_dot_comma.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int dot_count = 0;
  int comma_count = 0;
  int number_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);

    if (token->id == MINT_TOKEN_ID_DOT) {
      dot_count++;
    } else if (token->id == MINT_TOKEN_ID_COMMA) {
      comma_count++;
    } else if (token->id == MINT_TOKEN_ID_NUMBER) {
      number_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // We should have at least some dots, commas, and numbers
  RTL_TEST_TRUE(dot_count > 0);
  RTL_TEST_TRUE(comma_count > 0);
  RTL_TEST_TRUE(number_count > 0);

  // Check specific counts based on the test file
  RTL_TEST_EQUAL(dot_count, 6);  // 5 dots in member access + 1 standalone dot
  RTL_TEST_EQUAL(comma_count, 9);

  return 0;
}

RTL_TEST_FUNCTION(test_007_complex_numbers)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "007_complex_numbers.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count number tokens
  int number_token_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    if (token->id == MINT_TOKEN_ID_NUMBER) {
      number_token_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // We should have at least 10 numbers in the file
  RTL_TEST_TRUE(number_token_count >= 10);

  return 0;
}

RTL_TEST_FUNCTION(test_008_complex_expressions)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "008_complex_expressions.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int total_tokens = 0;
  int operator_count = 0;
  int assignment_op_count = 0;
  int comparison_op_count = 0;
  int paren_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    total_tokens++;

    if (token->id >= MINT_TOKEN_ID_PLUS && token->id <= MINT_TOKEN_ID_DIVIDE) {
      operator_count++;
    } else if (token->id >= MINT_TOKEN_ID_PLUS_ASSIGN && token->id <= MINT_TOKEN_ID_DIVIDE_ASSIGN) {
      assignment_op_count++;
    } else if (token->id >= MINT_TOKEN_ID_GREATER && token->id <= MINT_TOKEN_ID_NOTEQUAL) {
      comparison_op_count++;
    } else if (token->id == MINT_TOKEN_ID_LPAREN || token->id == MINT_TOKEN_ID_RPAREN) {
      paren_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // Verify we have sufficient tokens of each type
  RTL_TEST_TRUE(total_tokens > 50);
  RTL_TEST_TRUE(operator_count > 5);
  RTL_TEST_TRUE(assignment_op_count > 0);
  RTL_TEST_TRUE(comparison_op_count > 0);
  RTL_TEST_TRUE(paren_count > 0);

  return 0;
}

RTL_TEST_FUNCTION(test_009_language_constructs)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "009_language_constructs.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int total_tokens = 0;
  int keyword_count = 0;
  int identifier_count = 0;
  int string_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    total_tokens++;

    if (token->id >= MINT_TOKEN_ID_STRUCT && token->id <= MINT_TOKEN_ID_FALSE) {
      keyword_count++;
    } else if (token->id == MINT_TOKEN_ID_IDENTIFIER) {
      identifier_count++;
    } else if (token->id == MINT_TOKEN_ID_STRING) {
      string_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // Verify we have sufficient tokens of each type
  RTL_TEST_TRUE(total_tokens > 100);
  RTL_TEST_TRUE(keyword_count > 10);
  RTL_TEST_TRUE(identifier_count > 20);
  RTL_TEST_TRUE(string_count > 0);

  return 0;
}

RTL_TEST_FUNCTION(test_010_vec3)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "010_vec3.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int token_count = 0;
  int keyword_count = 0;
  int identifier_count = 0;
  int symbol_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    token_count++;

    if (token->id >= MINT_TOKEN_ID_STRUCT && token->id <= MINT_TOKEN_ID_FALSE) {
      keyword_count++;
    } else if (token->id == MINT_TOKEN_ID_IDENTIFIER) {
      identifier_count++;
    } else if (token->id >= MINT_TOKEN_ID_LBRACE && token->id <= MINT_TOKEN_ID_ARROW) {
      symbol_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // We should have some of each token type
  RTL_TEST_TRUE(token_count > 30);  // The Vec3 file has at least 30 tokens
  RTL_TEST_TRUE(keyword_count > 0);
  RTL_TEST_TRUE(identifier_count > 0);
  RTL_TEST_TRUE(symbol_count > 0);

  return 0;
}

RTL_TEST_FUNCTION(test_011_edge_cases)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "011_edge_cases.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int total_tokens = 0;
  int identifier_count = 0;
  int operator_count = 0;
  int string_count = 0;
  int number_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    total_tokens++;

    if (token->id == MINT_TOKEN_ID_IDENTIFIER) {
      identifier_count++;

      // Check that complex identifiers are handled correctly
      if (token->buffer_size > 0) {
        RTL_TEST_TRUE(token->buffer[0] == '_' ||
                      (token->buffer[0] >= 'a' && token->buffer[0] <= 'z') ||
                      (token->buffer[0] >= 'A' && token->buffer[0] <= 'Z'));
      }
    } else if (token->id >= MINT_TOKEN_ID_PLUS && token->id <= MINT_TOKEN_ID_ASSIGN) {
      operator_count++;
    } else if (token->id == MINT_TOKEN_ID_STRING) {
      string_count++;

      // Check that string content is properly captured
      RTL_TEST_TRUE(token->buffer_size > 0);
    } else if (token->id == MINT_TOKEN_ID_NUMBER) {
      number_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // Verify we have tokens of each type
  RTL_TEST_TRUE(total_tokens > 20);
  RTL_TEST_TRUE(identifier_count > 0);
  RTL_TEST_TRUE(operator_count > 0);
  RTL_TEST_TRUE(string_count > 0);
  RTL_TEST_TRUE(number_count > 0);

  return 0;
}

RTL_TEST_FUNCTION(test_012_comprehensive)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "012_comprehensive.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type
  int total_tokens = 0;
  int keyword_count = 0;
  int identifier_count = 0;
  int symbol_count = 0;
  int operator_count = 0;
  int number_count = 0;
  int string_count = 0;

  struct rtl_list_entry* it;
  rtl_list_for_each(it, &lexer.tokens)
  {
    struct mint_token* token = rtl_list_record(it, struct mint_token, link);
    total_tokens++;

    if (token->id >= MINT_TOKEN_ID_STRUCT && token->id <= MINT_TOKEN_ID_FALSE) {
      keyword_count++;
    } else if (token->id == MINT_TOKEN_ID_IDENTIFIER) {
      identifier_count++;
    } else if (token->id >= MINT_TOKEN_ID_LBRACE && token->id <= MINT_TOKEN_ID_ARROW) {
      symbol_count++;
    } else if (token->id >= MINT_TOKEN_ID_PLUS && token->id <= MINT_TOKEN_ID_ASSIGN) {
      operator_count++;
    } else if (token->id == MINT_TOKEN_ID_NUMBER) {
      number_count++;
    } else if (token->id == MINT_TOKEN_ID_STRING) {
      string_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // Verify we have sufficient tokens of each type
  RTL_TEST_TRUE(total_tokens > 300);
  RTL_TEST_TRUE(keyword_count > 10);
  RTL_TEST_TRUE(identifier_count > 50);
  RTL_TEST_TRUE(symbol_count > 50);
  RTL_TEST_TRUE(operator_count > 20);
  RTL_TEST_TRUE(number_count > 9);
  RTL_TEST_TRUE(string_count > 0);

  return 0;
}

int main(void)
{
  rtl_init();
  // Initialize the test framework
  rtl_test_init();

  // Run tests
  RTL_RUN_TEST(test_001_empty);
  RTL_RUN_TEST(test_002_comments);
  RTL_RUN_TEST(test_003_numbers);
  RTL_RUN_TEST(test_004_identifiers);
  RTL_RUN_TEST(test_005_symbols);
  RTL_RUN_TEST(test_006_dot_comma);
  RTL_RUN_TEST(test_007_complex_numbers);
  RTL_RUN_TEST(test_008_complex_expressions);
  RTL_RUN_TEST(test_009_language_constructs);
  RTL_RUN_TEST(test_010_vec3);
  RTL_RUN_TEST(test_011_edge_cases);
  RTL_RUN_TEST(test_012_comprehensive);

  // Print summary and return appropriate exit code
  int exit_code = rtl_test_summary();

  // Cleanup after all tests
  rtl_cleanup();

  return exit_code;
}
