#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr hundred = int_as_fistr(100);
  Fistr two_three_zero = int_as_fistr(100);
  
  Fistr sum = fistr_add(&hundred, &two_three_zero);
  print_fistr(&sum);
  
  for (size_t i = 0; i < 20; ++i) {
    sum = fistr_add(&sum, &two_three_zero);
    print_fistr(&sum);
  }
  
  return 0;
}
