#include <stdio.h>
#include <stdlib.h>

#include "mint_lexer.h"

#include <rtl_list.h>
#include <rtl_memory.h>
#include <rtl_test.h>

#include "rtl.h"

static struct mint_lexer lexer;

// Utility function to read a file content
static char* read_file_content(const char* file_path)
{
  FILE* file = fopen(file_path, "r");
  if (file == NULL) {
    fprintf(stderr, "Could not open test file: %s\n", file_path);
    return NULL;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  const long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate buffer and read file
  char* buffer = rtl_malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Memory allocation for file buffer failed\n");
    fclose(file);
    return NULL;
  }

  const size_t read_size = fread(buffer, 1, file_size, file);
  buffer[read_size] = '\0';

  fclose(file);
  return buffer;
}

static void setup(void)
{
  rtl_init();
  mint_lexer_init(&lexer);
}

static void teardown(void)
{
  mint_lexer_cleanup(&lexer);
  rtl_cleanup();
}

// Test for basic tokenization
RTL_TEST_FUNCTION(test_002_comments)
{
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

  RTL_TEST_EQUAL(token_count, 0);

  return 0;
}

int main(void)
{
  // Initialize the test framework
  rtl_test_init();

  // Setup for all tests
  setup();

  // Run tests
  RTL_RUN_TEST(test_002_comments);

  // Teardown after all tests
  teardown();

  // Print summary and return appropriate exit code
  return rtl_test_summary();
}
