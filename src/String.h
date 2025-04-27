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
void string_capacity_inc(String *string, size_t capacity_inc);
void string_print(String string);

void string_itoa(String *string);
void string_atoi(String *string);
String string_dup(String string);
void string_reverse(String *string);
void string_trim_left(String *string, size_t trim_size);
void string_trim_right(String *string, size_t trim_size);

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

void string_capacity_inc(String *string, size_t capacity_inc)
{
  String str = string_alloc(string->capacity + capacity_inc);
  for (size_t i = 0; i < string->size; ++i) {
    str.buffer[str.size++] = string->buffer[i];
  }

  free(string->buffer);
  *string = str;
}

void string_print(String string)
{
  for (size_t i = 0; i < string.size; ++i) {
    printf("%c", string.buffer[i]);
  }
  printf("\n");
}

void string_itoa(String *string)
{
  for (size_t i = 0; i < string->size; ++i) {
    assert(string->buffer[i] >= 0 && string->buffer[i] <= 9);
    string->buffer[i] += '0';
  }
}

void string_atoi(String *string)
{
  for (size_t i = 0; i < string->size; ++i) {
    assert(string->buffer[i] >= '0' && string->buffer[i] <= '9');
    string->buffer[i] -= '0';
  }
}

String string_dup(String string)
{
  String duplicate_string = string_alloc(string.capacity);
  for (size_t i = 0; i < string.size; ++i) {
    duplicate_string.buffer[duplicate_string.size++] = string.buffer[i];
  }
  
  return duplicate_string;
}

void string_reverse(String *string)
{
  for (size_t i = 0; i < string->size / 2; ++i) {
    char temp_ch = string->buffer[string->size - i - 1];
    string->buffer[string->size - i - 1] = string->buffer[i];
    string->buffer[i] = temp_ch;
  }
}

void string_trim_left(String *string, size_t trim_size)
{
  String str = string_alloc(string->capacity - trim_size);
  for (size_t i = trim_size; i < string->size; ++i) {
    str.buffer[str.size++] = string->buffer[i];
  }

  free(string->buffer);
  *string = str;
}

void string_trim_right(String *string, size_t trim_size)
{
  string->size -= trim_size;
}

#endif // STRING_IMPLEMENTATION
