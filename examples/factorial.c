#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr start = int_as_fistr(1);
  for (size_t i = 1; i <= 100; ++i) {
    Fistr next = int_as_fistr(i);
    fistr_mul(&start, &next);
    printf("%2zu: ", i);
    fistr_print(start);
    free(next.string.buffer);
  }

  free(start.string.buffer);
  
  return 0;
}
