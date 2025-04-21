#ifndef STRING_H_
#define STRING_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *buffer;
  size_t size;
  size_t capacity;
} String;

String string_alloc(size_t capacity);
void string_print(String string);

#endif // STRING_H_


#ifdef STRING_IMPLEMENTATION

String string_alloc(size_t capacity)
{
  return (String) {
    .buffer = (char *) malloc(sizeof(char) * capacity),
    .size = 0,
    .capacity = capacity
  };
}

void string_print(String string)
{
  for (size_t i = 0; i < string.size; ++i) {
    printf("%c", string.buffer[i]);
  }
  printf("\n");
}

#endif // STRING_IMPLEMENTATION
