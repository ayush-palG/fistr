#define FISTR_IMPLEMENTATION
#include "fistr.h"

int main(void)
{
  Fistr one_two_three = int_as_fistr(123);
  Fistr seventeen = int_as_fistr(17);
  
  Fistr result = fistr_mod(&one_two_three, &seventeen);
  print_fistr(&result);

  // for (size_t i = 0; i < 10; ++i) {
  //   result = fistr_div(&result, &two);
  //   print_fistr(&result);
  // }
  
  return 0;
}
