#include <rtl.h>

int main(const int argc, char* argv[])
{
  (void)argc;
  (void)argv;

  rtl_init();
  rtl_term();

  return 0;
}
