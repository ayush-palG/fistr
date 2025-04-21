#define STRING_IMPLEMENTATION
#include "String.h"

int main(void)
{
  String str = string_alloc(10);
  
  for (size_t i = 0; i < 5; ++i) {
    str.buffer[str.size++] = i + 'a';
  }

  string_print(str);
  string_trim_left(&str, 1);
  string_trim_right(&str, 1);
  string_print(str);
  
  return 0;
}
