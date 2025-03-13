#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr five_hundred_twelve = int_as_fistr(512);
  Fistr two = int_as_fistr(2);
  
  Fistr result = fistr_div(&five_hundred_twelve, &two);
  print_fistr(&result);

  for (size_t i = 0; i < 10; ++i) {
    result = fistr_div(&result, &two);
    print_fistr(&result);
  }
  
  return 0;
}
