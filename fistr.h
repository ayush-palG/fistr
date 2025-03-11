#ifndef FISTR_H_
#define FISTR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {
  INTEGER,
  FLOAT,
} Fistr_Type;

typedef enum {
  POSITIVE,
  NEGATIVE,
} Fistr_Sign;

typedef struct {
  char *str;
  size_t str_size;
  Fistr_Type type;
  Fistr_Sign sign;
} Fistr;

// integer as fistr
// float or double as fistr (maybe double)

void print_fistr(Fistr *fistr);
size_t int_len(int num);

Fistr int_as_fistr(int num);
Fistr double_as_fistr(double num);

void remove_leading_zeros_from_fistr(Fistr *fistr);
Fistr fistr_add(Fistr *addend_1, Fistr *addend_2);

#endif // FISTR_H_

#ifdef FISTR_IMPLEMENTATION

void print_fistr(Fistr *fistr)
{
  if (fistr->sign == NEGATIVE) printf("-");
  for (size_t i = 0; fistr->str[i] != 0; ++i) printf("%c", fistr->str[i]);
}

size_t int_len(int num)
{
  if (num == 0) return 1;
  size_t length = 0;
  while (num != 0) {
    length += 1;
    num /= 10;
  }
  return length;
}

Fistr int_as_fistr(int num)
{
  size_t int_length = int_len(num);
  char *str = (char *) malloc(sizeof(char) * (int_length + 1));
  for (size_t i = 0; i < int_length; ++i) str[i] = '0';
  str[int_length] = 0;

  Fistr_Sign sign;
  if (num >= 0) sign = POSITIVE;
  else sign = NEGATIVE;
  
  for (size_t i = int_length - 1; i < int_length; --i) {
    str[i] = '0' + abs(num % 10);
    num /= 10;
  }

  return (Fistr) {.str = str, .str_size = int_length, .type = INTEGER, .sign = sign};
}

void remove_leading_zeros_from_fistr(Fistr *fistr)
{
  size_t i = 0;
  while (fistr->str[i] == '0') i += 1;
  if (fistr->str_size == i) return;
  
  char *tmp_fistr_str = fistr->str;
  char *new_str = (char *) malloc(sizeof(char) * (fistr->str_size-i+1));
  memcpy(new_str, fistr->str + i, fistr->str_size-i+1);
  fistr->str = new_str;
  free(tmp_fistr_str);
}

Fistr fistr_add(Fistr *addend_1, Fistr *addend_2)
{
  // Now I am assuming that both addend types are integers
  // I am also assuming that both numbers are positive
  size_t carry = 0;
  size_t str_size = (addend_1->str_size > addend_2->str_size) ? addend_1->str_size : addend_2->str_size;
  char *str = (char *) malloc(sizeof(char) * (str_size + 2));
  for (size_t i = 0; i <= str_size; ++i) str[i] = '0';
  str[str_size+1] = 0;
  
  for (size_t i = str_size - 1; i < str_size; --i) {
    size_t sum = (addend_1->str[i] - '0') + (addend_2->str[i] - '0') + carry;
    carry = sum / 10;
    str[i+1] = (sum % 10) + '0';
  }
  str[0] = carry + '0';
  
  Fistr total_sum = {.str = str, .str_size = str_size+1, .type = INTEGER, .sign = POSITIVE};
  remove_leading_zeros_from_fistr(&total_sum);
  return total_sum;
}

#if 0
Fistr fistr_mult(Fistr *multiplicand, Fistr *multiplier)
{
  assert(0 && "fistr_mult() not implemented\n");
}

#define MANTISSA_MASK ((1LL << 42LL) - 1LL)

size_t double_len(double num)
{
  assert(0 && "double_len() not implemented\n");
}

Fistr double_as_fistr(double num)
{
  assert(0 && "double_as_fistr() not implemented\n");
}
#endif

#endif // FISTR_IMPLEMENTATION
