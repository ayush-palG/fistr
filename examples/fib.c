#define FISTR_IMPLEMENTATION
#include "../fistr.h"

int main(void)
{
  Fistr a = int_as_fistr(0);
  Fistr b = int_as_fistr(1);
  Fistr c;
  
  for (size_t i = 0; i < 100; ++i) {
    c = fistr_add(&a, &b);
    free(a.str);
    a = b;
    b = c;
    print_fistr(&b);
  }

  free(b.str);
  free(a.str);
}
