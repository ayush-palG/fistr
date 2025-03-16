#define FISTR_IMPLEMENTATION
#include "../fistr.h"

int main(void)
{
  arena_init(&arena, 1024*10);
  
  Fistr a = int_as_fistr(0);
  Fistr b = int_as_fistr(1);
  Fistr c;
  
  for (size_t i = 0; i < 10; ++i) {
    c = fistr_add(a, b);
    a = b;
    b = c;
    print_fistr(b);
  }

  arena_delete(&arena);
}
