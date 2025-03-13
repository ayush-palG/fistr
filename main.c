#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr fifty = int_as_fistr(50);
  Fistr two = int_as_fistr(2);
  
  Fistr sum = fistr_mult(&fifty, &two);
  print_fistr(&sum);

  for (size_t i = 0; i < 10; ++i) {
    sum = fistr_mult(&sum, &two);
    print_fistr(&sum);
  }
  
  return 0;
}
