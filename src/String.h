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
String string_from_cstr(const char *cstr);

void string_capacity_inc(String *string, size_t capacity_inc);
void string_print(String string);

void string_itoa(String *string);
void string_atoi(String *string);

String string_dup(String string);
String string_slice(String string, size_t start, size_t end);
// TODO: See if we need to return String or void
String string_rep(String string, size_t rep_count);

void string_concat(String *string1, String *string2);
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

String string_from_cstr(const char *cstr)
{
  size_t cstr_len = 0;
  while (cstr[cstr_len] != '\0') cstr_len += 1;

  String string = string_alloc(cstr_len);
  for (size_t i = 0; i < cstr_len; ++i) {
    string.buffer[string.size++] = cstr[i];
  }

  return string;
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

String string_slice(String string, size_t start, size_t end)
{
  assert(start <= end);
  assert(end <= string.size);
  String slice = string_alloc(end - start);
  for (size_t i = start; i < end; ++i) {
    slice.buffer[slice.size++] = string.buffer[i];
  }
  return slice;
}

String string_rep(String string, size_t rep_count)
{
  String rep_string = string_alloc(string.size * rep_count);
  for (size_t i = 0; i < rep_string.capacity; ++i) {
    rep_string.buffer[rep_string.size++] = string.buffer[i%string.size];
  }

  return rep_string;
}

void string_concat(String *string1, String *string2)
{
  if (string1->capacity < string1->size + string2->size) {
    string_capacity_inc(string1, string2->size);
  }
  
  for (size_t i = 0; i < string2->size; ++i) {
    string1->buffer[string1->size++] = string2->buffer[i];
  }
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
