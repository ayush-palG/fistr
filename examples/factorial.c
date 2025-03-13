#define FISTR_IMPLEMENTATION
#include "../fistr.h"

int main(void)
{
  Fistr a, b, c;

  b = int_as_fistr(1);
  for (size_t i = 1; i <= 10; ++i) {
    a = int_as_fistr(i);
    c = b;
    b = fistr_mult(&b, &a);
    print_fistr(&b);
    free(a.str);
    free(c.str);
  }

  free(b.str);
  return 0;
}
