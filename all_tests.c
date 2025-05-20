#include <stdio.h>
#include <stdlib.h>

#include "mint_lexer.h"

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

int main(void)
{
  rtl_init();
  // Initialize the test framework
  rtl_test_init();

  // Run tests
  RTL_RUN_TEST(test_001_empty);
  RTL_RUN_TEST(test_002_comments);

  // Cleanup after all tests
  rtl_cleanup();

  // Print summary and return appropriate exit code
  return rtl_test_summary();
}
