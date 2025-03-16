#ifndef FISTR_H_
#define FISTR_H_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_IMPLEMENTATION
#include "arena.h"

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

void print_fistr(const Fistr fistr);
size_t int_len(int num);
char *alloc_n_chars(size_t n);

Fistr int_as_fistr(int num);
Fistr double_as_fistr(double num);

void remove_leading_zeros_from_fistr(Fistr *fistr);
void add_leading_zeros_to_fistr(Fistr *fistr, size_t num_of_zeros);
Fistr get_larger_fistr(Fistr fistr_1, Fistr fistr_2);
bool are_fistr_equal(Fistr fistr_1, Fistr fistr_2);

Fistr fistr_left_shift(Fistr fistr, size_t operand);
Fistr fistr_right_shift(Fistr fistr, size_t operand);
Fistr fistr_mult_by_st(Fistr fistr, size_t operand);
Fistr fistr_dup(Fistr fistr);

Fistr fistr_add(Fistr operand_1, Fistr operand_2);
Fistr fistr_mult(Fistr operand_1, Fistr operand_2);
Fistr fistr_div(Fistr operand_1, Fistr operand_2);
Fistr fistr_mod(Fistr operand_1, Fistr operand_2);

#endif // FISTR_H_


#ifdef FISTR_IMPLEMENTATION

void print_fistr(const Fistr fistr)
{
  if (fistr.sign == NEGATIVE) printf("-");
  for (size_t i = 0; fistr.str[i] != 0; ++i) printf("%c", fistr.str[i]);
  printf("\n");
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

char *alloc_n_chars(size_t n)
{
  // printf("%ld\n", arena.current_offset);
  return arena_alloc(&arena, n, sizeof(char), _Alignof(char));
}

Fistr int_as_fistr(int num)
{
  size_t int_length = int_len(num);
  char *str = alloc_n_chars(int_length + 1);
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
  
  char *new_str;
  if (fistr->str_size == i) {
    new_str = alloc_n_chars(2);
    new_str[0] = '0';
    new_str[1] = 0;
  } else {
    new_str = alloc_n_chars(fistr->str_size-i+1);
    memcpy(new_str, fistr->str + i, fistr->str_size-i+1);
  }
  fistr->str = new_str;

  if (fistr->str_size == i) { // when all digits are 0
    fistr->str_size = 1;
    fistr->sign = POSITIVE;
  }
  else fistr->str_size -= i;
}

void add_leading_zeros_to_fistr(Fistr *fistr, size_t num_of_zeros)
{
  char *new_str = alloc_n_chars(fistr->str_size+1 + num_of_zeros);
  for (size_t i = 0; i < num_of_zeros; ++i) new_str[i] = '0';
  memcpy(new_str + num_of_zeros, fistr->str, fistr->str_size+1);
  fistr->str = new_str;
}

Fistr get_larger_fistr(Fistr fistr_1, Fistr fistr_2)
{
  assert(fistr_1.str_size == fistr_2.str_size);
  for (size_t i = 0; i < fistr_1.str_size; ++i) {
    if (fistr_1.str[i] != fistr_2.str[i]) {
      if (fistr_1.str[i] > fistr_2.str[i]) return fistr_1;
      else if (fistr_1.str[i] < fistr_2.str[i]) return fistr_2;
    }
  }
  return fistr_1;
}

bool are_fistr_equal(Fistr fistr_1, Fistr fistr_2)
{
  if (fistr_1.sign != fistr_2.sign) return false;
  if (fistr_1.str_size != fistr_2.str_size) return false;
  for (size_t i = 0; i < fistr_1.str_size; ++i) {
    if (fistr_1.str[i] != fistr_2.str[i]) return false;
  }
  return true;
}

Fistr fistr_add(Fistr operand_1, Fistr operand_2)
{
  size_t str_size;
  if (operand_1.str_size == operand_2.str_size) {
    str_size = operand_1.str_size;
  } else if (operand_1.str_size > operand_2.str_size) {
    str_size = operand_1.str_size;
    add_leading_zeros_to_fistr(&operand_2, (size_t) (operand_1.str_size - operand_2.str_size));
    operand_2.str_size += (operand_1.str_size - operand_2.str_size);
  } else if (operand_1.str_size < operand_2.str_size) {
    str_size = operand_2.str_size;
    add_leading_zeros_to_fistr(&operand_1, (size_t) (operand_2.str_size - operand_1.str_size));
    operand_1.str_size += (operand_2.str_size - operand_1.str_size);
  }

  Fistr_Sign sign;
  
  if (operand_1.sign == POSITIVE && operand_2.sign == POSITIVE) sign = POSITIVE;
  else if (operand_1.sign == NEGATIVE && operand_2.sign == NEGATIVE) sign = NEGATIVE;
  
  char *str = alloc_n_chars(str_size + 2);
  for (size_t i = 0; i <= str_size; ++i) str[i] = '0';
  str[str_size+1] = 0;

  if ((operand_1.sign == POSITIVE && operand_2.sign == POSITIVE)
      || (operand_1.sign == NEGATIVE && operand_2.sign == NEGATIVE)) {
    size_t carry = 0;
    for (size_t i = str_size - 1; i < str_size; --i) {
      size_t sum = (operand_1.str[i] - '0') + (operand_2.str[i] - '0') + carry;
      carry = sum / 10;
      str[i+1] = (sum % 10) + '0';
    }
    str[0] = carry + '0';
  } else {
    size_t borrow = 0;
    Fistr larger_fistr = get_larger_fistr(operand_1, operand_2);
    if (are_fistr_equal(larger_fistr, operand_1)) {
      for (size_t i = str_size - 1; i < str_size; --i) {
	size_t difference = (operand_1.str[i] - borrow) - operand_2.str[i];
	if (difference <= 9) borrow = 0;
	else borrow = 1;
	str[i+1] = difference + (10*borrow) + '0';
      }
      sign = operand_1.sign;
    } else {
      for (size_t i = str_size - 1; i < str_size; --i) {
	size_t difference = (operand_2.str[i] - borrow) - operand_1.str[i];
	if (difference <= 9) borrow = 0;
	else borrow = 1;
	str[i+1] = difference + (10*borrow) + '0';
      }
      sign = operand_2.sign;
    }
  }
  
  Fistr result = {.str = str, .str_size = str_size+1, .type = INTEGER, .sign = sign};
  remove_leading_zeros_from_fistr(&result);
  return result;
}

Fistr fistr_left_shift(Fistr fistr, size_t operand)
{
  char *str = alloc_n_chars(fistr.str_size + 1 + operand);
  memcpy(str, fistr.str, fistr.str_size);
  for (size_t i = fistr.str_size; i < fistr.str_size + operand; ++i) str[i] = '0';
  str[fistr.str_size+operand] = 0;
  
  Fistr result = {.str = str, .str_size = fistr.str_size+operand, .type = fistr.type, .sign = fistr.sign};
  remove_leading_zeros_from_fistr(&result);
  return result;
}

Fistr fistr_right_shift(Fistr fistr, size_t operand)
{
  char *str = alloc_n_chars(fistr.str_size + 1 - operand);
  for (size_t i = 0; i < fistr.str_size - operand; ++i) str[i] = fistr.str[i];
  str[fistr.str_size-operand] = 0;

  Fistr result = {.str = str, .str_size = fistr.str_size-operand, .type = fistr.type, .sign = fistr.sign};
  remove_leading_zeros_from_fistr(&result);
  return result;
}

Fistr fistr_mult_by_st(Fistr fistr, size_t operand)
{
  // TODO: Operand in 1-digit number as of now
  char *str = alloc_n_chars(fistr.str_size + 2);
  for (size_t i = 0; i <= fistr.str_size; ++i) str[i] = '0';
  str[fistr.str_size+1] = 0;
  
  size_t carry = 0;
  for (size_t i = fistr.str_size - 1; i < fistr.str_size; --i) {
    size_t num = ((fistr.str[i] - '0') * operand) + carry;
    carry = num / 10;
    str[i+1] = (num % 10) + '0';
  }
  str[0] = (carry % 10) + '0';

  Fistr result = {.str = str, .str_size = fistr.str_size+1, .type = INTEGER, .sign = POSITIVE};
  remove_leading_zeros_from_fistr(&result);
  return result;
}

Fistr fistr_dup(Fistr fistr)
{
  char *str = alloc_n_chars(fistr.str_size + 1);
  for (size_t i = 0; i < fistr.str_size; ++i) str[i] = fistr.str[i];
  str[fistr.str_size] = 0;

  Fistr result = {.str = str, .str_size = fistr.str_size, .type = fistr.type, .sign = fistr.sign};
  return result;
}

Fistr fistr_mult(Fistr operand_1, Fistr operand_2)
{
  Fistr_Sign sign;
  if ((operand_1.sign == POSITIVE && operand_2.sign == POSITIVE)
      || (operand_1.sign == NEGATIVE && operand_2.sign == NEGATIVE)) sign = POSITIVE;
  else sign = NEGATIVE;

  Fistr result = int_as_fistr(0);
  for (size_t i = 0; i < operand_2.str_size; ++i) {
    Fistr sum = fistr_mult_by_st(operand_1, operand_2.str[i] - '0');
    result = fistr_add(result, sum);
    result = fistr_left_shift(result, 1);
  }

  result = fistr_right_shift(result, 1);
  result.sign = sign;
  return result;
}

Fistr fistr_div(Fistr operand_1, Fistr operand_2)
{
  Fistr_Sign sign;
  if ((operand_1.sign == POSITIVE && operand_2.sign == POSITIVE)
      || (operand_1.sign == NEGATIVE && operand_2.sign == NEGATIVE)) sign = POSITIVE;
  else sign = NEGATIVE;

  operand_1.sign = POSITIVE;
  operand_2.sign = NEGATIVE;
  
  Fistr quotient = int_as_fistr(0);
  while (1) {
    operand_1 = fistr_add(operand_1, operand_2);
    if (operand_1.sign == NEGATIVE) break;
    quotient = fistr_add(quotient, int_as_fistr(1));
  }

  quotient.sign = sign;
  return quotient;
}

Fistr fistr_mod(Fistr operand_1, Fistr operand_2)
{
  // dividend % divisor = (dividend - divisor * (dividend / divisor))
  // But the above operation is more expensive than simple division we used
  // And the sign of mod is always positive or zero
  
  Fistr dividend = fistr_dup(operand_1);
  dividend.sign = POSITIVE;
  operand_2.sign = NEGATIVE;
  
  while (dividend.sign == POSITIVE) {
    dividend = fistr_add(dividend, operand_2);
  }

  operand_2.sign = POSITIVE;
  dividend = fistr_add(dividend, operand_2);

  return dividend;
}

#if 0
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
