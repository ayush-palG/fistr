#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  // TODO: Instead of giving integer as an argument, give string
  Fistr two = int_as_fistr(2);
  Fistr thousand = int_as_fistr(1000);

  fistr_print(two);
  fistr_print(thousand);
  fistr_pow(&two, &thousand);

  printf("\n");
  fistr_print(two);
  fistr_print(thousand);

  // TODO: Introduce fistr_free and string_free
  free(two.string.buffer);
  free(thousand.string.buffer);
  
  return 0;
}
