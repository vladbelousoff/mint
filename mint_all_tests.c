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

RTL_TEST_FUNCTION(test_005_all_token_types)
{
  struct mint_lexer lexer;
  mint_lexer_init(&lexer);

  char file_path[256];
  snprintf(file_path, sizeof(file_path), "%s/%s", MINT_TEST_DIR, "005_all_token_types.mint");

  char* content = read_file_content(file_path);
  RTL_TEST_NOT_NULL(content);

  mint_lexer_tokenize(&lexer, content);

  // Count tokens by type - comprehensive test for all token types
  int total_tokens = 0;
  int symbol_count = 0;
  int operator_count = 0;
  int string_count = 0;
  int number_count = 0;
  int identifier_count = 0;
  int keyword_count = 0;

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
    } else if (token->id == MINT_TOKEN_ID_NUMBER) {
      number_count++;
    } else if (token->id == MINT_TOKEN_ID_IDENTIFIER) {
      identifier_count++;
    } else if (token->id >= MINT_TOKEN_ID_STRUCT && token->id <= MINT_TOKEN_ID_FALSE) {
      keyword_count++;
    }
  }

  rtl_free(content);
  mint_lexer_cleanup(&lexer);

  // Verify we have comprehensive coverage of all token types
  RTL_TEST_TRUE(total_tokens > 100);     // Should have many tokens in comprehensive test
  RTL_TEST_TRUE(symbol_count >= 11);     // All 11 symbol types should be present
  RTL_TEST_TRUE(operator_count >= 15);   // All 15 operator types should be present
  RTL_TEST_TRUE(string_count >= 3);      // Should have multiple strings
  RTL_TEST_TRUE(number_count >= 8);      // Should have multiple numbers
  RTL_TEST_TRUE(identifier_count >= 5);  // Should have multiple identifiers
  RTL_TEST_TRUE(keyword_count >= 14);    // All 14 keyword types should be present

  return 0;
}

int main(void)
{
  rtl_init();
  // Initialize the test framework
  rtl_test_init();

  // Run tests - only 001-005 as requested
  RTL_RUN_TEST(test_001_empty);
  RTL_RUN_TEST(test_002_comments);
  RTL_RUN_TEST(test_003_numbers);
  RTL_RUN_TEST(test_004_identifiers);
  RTL_RUN_TEST(test_005_all_token_types);

  // Print summary and return appropriate exit code
  int exit_code = rtl_test_summary();

  // Cleanup after all tests
  rtl_cleanup();

  return exit_code;
}
