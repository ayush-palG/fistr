#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr hundred = int_as_fistr(100);
  Fistr two_three_zero = int_as_fistr(230);
  
  Fistr sum = fistr_add(&hundred, &two_three_zero);
  print_fistr(&sum);
  
  return 0;
}
