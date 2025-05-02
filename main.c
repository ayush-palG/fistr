#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  // TODO: Instead of giving integer as an argument, give string
  Fistr six_nums = int_as_fistr(123456);
  Fistr seventeen = int_as_fistr(17);

  fistr_print(six_nums);
  fistr_print(seventeen);
  fistr_mod(&six_nums, &seventeen);

  printf("\n");
  fistr_print(six_nums);
  fistr_print(seventeen);

  // TODO: Introduce fistr_free and string_free
  free(six_nums.string.buffer);
  free(seventeen.string.buffer);
  
  return 0;
}
