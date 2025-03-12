#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr four_hundred = int_as_fistr(400);
  Fistr minus_one_two_three = int_as_fistr(-123);
  
  Fistr sum = fistr_add(&four_hundred, &minus_one_two_three);
  print_fistr(&sum);

  for (size_t i = 0; i < 10; ++i) {
    sum = fistr_add(&sum, &minus_one_two_three);
    print_fistr(&sum);
  }
  
  return 0;
}
