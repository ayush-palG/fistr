#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr first  = int_as_fistr(1);
  Fistr second = int_as_fistr(1);
  Fistr temp;
  // 1 1 2 3 5 8 13 21 34 55

  for (size_t i = 0; i < 10; ++i) {
    fistr_print(first);
    temp = int_as_fistr(0);
    fistr_sum(&temp, &first);
    fistr_sum(&temp, &second);
    
    free(first.string.buffer);
    first = second;
    second = temp;
  }

  free(first.string.buffer);
  free(second.string.buffer);
}
