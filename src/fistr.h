#ifndef FISTR_H_
#define FISTR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define STRING_IMPLEMENTATION
#include "String.h"

typedef enum {
  INTEGER,
  FLOAT,
} Fistr_Type;

typedef enum {
  POSITIVE,
  NEGATIVE,
} Fistr_Sign;

typedef struct {
  String string;
  Fistr_Sign sign;
} Fistr;

void fistr_print(Fistr fistr);
size_t int_len(int num);

Fistr int_as_fistr(int num);
Fistr string_as_fistr(String string, Fistr_Sign sign);
Fistr fistr_dup(Fistr fistr);

void remove_leading_zeros_from_fistr(Fistr *fistr);
void add_leading_zeros_to_fistr(Fistr *fistr, size_t num_of_zeros);

void fistr_left_shift(Fistr *fistr, size_t operand);
void fistr_right_shift(Fistr *fistr, size_t operand);

// TODO: Implement fistr_swap
void fistr_swap(Fistr *dst, Fistr *src);
void fistr_mul_by_ten_multiple(Fistr *fistr, size_t multiple);

bool fistr_comp_sign(Fistr fistr1, Fistr fistr2);
int fistr_comp_size(Fistr fistr1, Fistr fistr2);
int fistr_comp_magnitude(Fistr fistr1, Fistr fistr2);

// TODO: Maybe introduce some more operations like modulus, exponentiation(power), logarithms 
void fistr_sum(Fistr *fistr1, Fistr *fistr2);
void fistr_sub(Fistr *fistr1, Fistr *fistr2);
void fistr_mul(Fistr *fistr1, Fistr *fistr2);
void fistr_div(Fistr *fistr1, Fistr *fistr2);
void fistr_mod(Fistr *fistr1, Fistr *fistr2);

void fistr_long_division(Fistr *fistr1, Fistr *fistr2, Fistr *quotient, Fistr *remainder);
void fistr_pow(Fistr *fistr1, Fistr *fistr2);

#endif // FISTR_H_


#ifdef FISTR_IMPLEMENTATION

void fistr_print(Fistr fistr)
{
  if (fistr.sign == NEGATIVE) printf("-");
  string_itoa(&fistr.string);
  string_print(fistr.string);
  string_atoi(&fistr.string);
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
  String string = string_alloc(int_len(num));

  Fistr_Sign sign;
  if (num >= 0) sign = POSITIVE;
  else sign = NEGATIVE;

  // TODO: Here, we could do num = abs(num);
  for (size_t i = string.capacity - 1; i < string.capacity; --i) {
    string.buffer[i] = abs(num % 10);
    num /= 10;
    string.size += 1;
  }

  return (Fistr) {.string = string, .sign = sign};
}

Fistr string_as_fistr(String string, Fistr_Sign sign)
{
  return (Fistr) {.string = string, .sign = sign};
}

Fistr fistr_dup(Fistr fistr)
{
  String string = string_alloc(fistr.string.capacity);

  for (size_t i = 0; i < fistr.string.size; ++i) {
    string.buffer[string.size++] = fistr.string.buffer[i];
  }
  
  return (Fistr) {.string = string, .sign = fistr.sign};
}

void remove_leading_zeros_from_fistr(Fistr *fistr)
{
  size_t i = 0;
  while (i < fistr->string.size && fistr->string.buffer[i] == 0) i += 1;
  
  if (fistr->string.size == i) {
    string_trim_left(&fistr->string, i-1);
    fistr->sign = POSITIVE;
  } else {
    string_trim_left(&fistr->string, i);
  }
}

void add_leading_zeros_to_fistr(Fistr *fistr, size_t num_of_zeros)
{
  String string = string_alloc(fistr->string.capacity + num_of_zeros);
  for (size_t i = 0; i < num_of_zeros; ++i) {
    string.buffer[string.size++] = 0;
  }
  for (size_t i = 0; i < fistr->string.size; ++i) {
    string.buffer[string.size++] = fistr->string.buffer[i];
  }
  free(fistr->string.buffer);
  fistr->string = string;
}

void fistr_left_shift(Fistr *fistr, size_t operand)
{
  assert(operand <= fistr->string.size);
  string_trim_left(&fistr->string, operand);
  string_capacity_inc(&fistr->string, operand);

  assert(fistr->string.size + operand <= fistr->string.capacity);
  for (size_t i = 0; i < operand; ++i) {
    fistr->string.buffer[fistr->string.size++] = 0;
  }
}

void fistr_right_shift(Fistr *fistr, size_t operand)
{
  assert(operand <= fistr->string.size);
  string_trim_right(&fistr->string, operand);
  string_reverse(&fistr->string);
  for (size_t i = 0; i < operand; ++i) {
    fistr->string.buffer[fistr->string.size++] = 0;
  }
  string_reverse(&fistr->string);
}

void fistr_mul_by_ten_multiple(Fistr *fistr, size_t multiple)
{
  // TODO: increase by (fistr->string.size - fistr->string.capacity) + multiple
  if (fistr->string.size >= fistr->string.capacity) {
    string_capacity_inc(&fistr->string, multiple);
  }
  for (size_t i = 0; i < multiple; ++i) {
    fistr->string.buffer[fistr->string.size++] = 0;
  }
}

bool fistr_comp_sign(Fistr fistr1, Fistr fistr2)
{
  if ((fistr1.sign == POSITIVE && fistr2.sign == POSITIVE)
      || (fistr1.sign == NEGATIVE && fistr2.sign == NEGATIVE)) return true;
  else return false;
}

int fistr_comp_size(Fistr fistr1, Fistr fistr2)
{
  return (int) fistr1.string.size - (int) fistr2.string.size;
}

int fistr_comp_magnitude(Fistr fistr1, Fistr fistr2)
{
  // If fistr1 is larger than fistr2 in magnitude then return 1 and -1 if vice-versa
  // Return 0 if both are equal in magnitude
  if (fistr_comp_size(fistr1, fistr2) == 0) {
    for (size_t i = 0; i < fistr1.string.size; ++i) {
      if (fistr1.string.buffer[i] > fistr2.string.buffer[i])      return 1;
      else if (fistr1.string.buffer[i] < fistr2.string.buffer[i]) return -1;
    }
    return 0;
  } else if (fistr_comp_size(fistr1, fistr2) > 0) {
    return 1;
  } else {
    return -1;
  }
}

void fistr_sum(Fistr *fistr1, Fistr *fistr2)
{
  if (fistr1->string.size >= fistr1->string.capacity) {
    string_capacity_inc(&fistr1->string, 1);
  }

  if (fistr_comp_size(*fistr1, *fistr2) < 0) {
    add_leading_zeros_to_fistr(fistr1, fistr2->string.size - fistr1->string.size);
  } else if (fistr_comp_size(*fistr1, *fistr2) > 0) {
    add_leading_zeros_to_fistr(fistr2, fistr1->string.size - fistr2->string.size);
  }
    
  if (fistr_comp_sign(*fistr1, *fistr2) == true) {
    uint8_t carry = 0;
    for (size_t i = fistr1->string.size-1; i < fistr1->string.size; --i) {
      fistr1->string.buffer[i] += fistr2->string.buffer[i] + carry;
      if (fistr1->string.buffer[i] > 9) {
	carry = 1;
	fistr1->string.buffer[i] %= 10;
      } else {
	carry = 0;
      }
    }
    if (carry == 1) {
      add_leading_zeros_to_fistr(fistr1, 1);
      fistr1->string.buffer[0] = 1;
    }
  } else {
    uint8_t borrow = 0;
    Fistr larger_fistr  = (fistr_comp_magnitude(*fistr1, *fistr2) >= 0) ? *fistr1 : *fistr2;
    Fistr smaller_fistr = (fistr_comp_magnitude(*fistr1, *fistr2) <  0) ? *fistr1 : *fistr2;
    
    for (size_t i = fistr1->string.size - 1; i < fistr1->string.size; --i) {
      size_t difference = (larger_fistr.string.buffer[i] - borrow) - smaller_fistr.string.buffer[i];
      if (difference <= 9) borrow = 0;
      else borrow = 1;
      fistr1->string.buffer[i] = difference + (10*borrow);
    }
    fistr1->sign = larger_fistr.sign;
  }
  
  remove_leading_zeros_from_fistr(fistr1);
  remove_leading_zeros_from_fistr(fistr2);
}

void fistr_sub(Fistr *fistr1, Fistr *fistr2)
{
  fistr2->sign = (fistr2->sign == POSITIVE) ? NEGATIVE : POSITIVE;
  fistr_sum(fistr1, fistr2);
  fistr2->sign = (fistr2->sign == POSITIVE) ? NEGATIVE : POSITIVE;
}

void fistr_mul(Fistr *fistr1, Fistr *fistr2)
{
  fistr1->sign = (fistr_comp_sign(*fistr1, *fistr2) == true) ? POSITIVE : NEGATIVE;

  Fistr fistr1_dup = fistr_dup(*fistr1);
  fistr1->string.size = 0;
  
  for (size_t i = fistr2->string.size-1; i < fistr2->string.size; --i) {
    for (size_t j = 0; j < (size_t) fistr2->string.buffer[i]; ++j) {
      fistr_sum(fistr1, &fistr1_dup);
    }
    fistr_mul_by_ten_multiple(&fistr1_dup, 1);
  }
  remove_leading_zeros_from_fistr(fistr1);

  free(fistr1_dup.string.buffer);
}

void fistr_div(Fistr *fistr1, Fistr *fistr2)
{
  fistr1->sign = (fistr_comp_sign(*fistr1, *fistr2) == true) ? POSITIVE : NEGATIVE;

  Fistr remainder = string_as_fistr(string_alloc(fistr1->string.size), POSITIVE);
  Fistr quotient  = string_as_fistr(string_alloc(fistr1->string.size), POSITIVE);

  fistr_long_division(fistr1, fistr2, &quotient, &remainder);

  free(fistr1->string.buffer);
  free(remainder.string.buffer);
  
  fistr1->string = quotient.string;
  remove_leading_zeros_from_fistr(fistr1);
}

void fistr_mod(Fistr *fistr1, Fistr *fistr2)
{
  fistr1->sign = (fistr_comp_sign(*fistr1, *fistr2) == true) ? POSITIVE : NEGATIVE;

  Fistr remainder = string_as_fistr(string_alloc(fistr1->string.size), POSITIVE);
  Fistr quotient  = string_as_fistr(string_alloc(fistr1->string.size), POSITIVE);

  fistr_long_division(fistr1, fistr2, &quotient, &remainder);

  free(fistr1->string.buffer);
  free(quotient.string.buffer);
  
  fistr1->string = remainder.string;
  remove_leading_zeros_from_fistr(fistr1);
}

void fistr_long_division(Fistr *fistr1, Fistr *fistr2, Fistr *quotient, Fistr *remainder)
{
  for (size_t i = 0; i < fistr1->string.size; ++i) {
    Fistr slice = string_as_fistr(string_slice(fistr1->string, i, i+1), POSITIVE);
    string_concat(&remainder->string, &slice.string);
    
    if (fistr_comp_magnitude(*fistr1, *fistr2) >= 0) {
      for (size_t i = 10; i > 0; --i) {
	Fistr temp = int_as_fistr(i);
	fistr_mul(&temp, fistr2);
	if (fistr_comp_magnitude(temp, *remainder) <= 0) {
	  fistr_sub(remainder, &temp);
	  quotient->string.buffer[quotient->string.size++] = i;
	  free(temp.string.buffer);
	  break;
	}
	free(temp.string.buffer);
      }
    } else {
      quotient->string.buffer[quotient->string.size++] = 0;
    }
    free(slice.string.buffer);
  }
}

void fistr_pow(Fistr *fistr1, Fistr *fistr2)
{
  if (fistr2->sign == POSITIVE) {
    // fistr1 ^ fistr2
    Fistr fistr1_dup = fistr_dup(*fistr1);
    Fistr one = int_as_fistr(1);
    fistr_sub(fistr2, &one);
    for (size_t i = fistr2->string.size-1; i < fistr2->string.size; --i) {
      for (size_t j = 0; j < (size_t) fistr2->string.buffer[i]; ++j) {
	fistr_mul(fistr1, &fistr1_dup);
      }
    }
    fistr_sum(fistr2, &one);
    free(one.string.buffer);
    free(fistr1_dup.string.buffer);
    remove_leading_zeros_from_fistr(fistr1);
  } else {
    // (1/fistr1) ^ fistr2
  }
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
