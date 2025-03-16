#define FISTR_IMPLEMENTATION
#include "../fistr.h"

int main(void)
{
  arena_init(&arena, 1024*1024);
  Fistr a, b;

  b = int_as_fistr(1);
  for (size_t i = 1; i <= 100; ++i) {
    a = int_as_fistr(i);
    b = fistr_mult(b, a);
    print_fistr(b);
  }

  return 0;
}
