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

void print_fistr(Fistr *fistr);
size_t int_len(int num);

Fistr int_as_fistr(int num);
Fistr double_as_fistr(double num);

void remove_leading_zeros_from_fistr(Fistr *fistr);
void add_leading_zeros_to_fistr(Fistr *fistr, size_t num_of_zeros);
size_t get_str_size_of_operands(Fistr *operand_1, Fistr *operand_2);
Fistr *get_larger_fistr(Fistr *fistr_1, Fistr *fistr_2);

Fistr fistr_add(Fistr *addend_1, Fistr *addend_2);

#endif // FISTR_H_


#ifdef FISTR_IMPLEMENTATION

void print_fistr(Fistr *fistr)
{
  if (fistr->sign == NEGATIVE) printf("-");
  for (size_t i = 0; fistr->str[i] != 0; ++i) printf("%c", fistr->str[i]);
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
  
  char *tmp_fistr_str = fistr->str;
  char *new_str;
  if (fistr->str_size == i) {
    new_str = (char *) malloc(sizeof(char) * (2));
    new_str[0] = '0';
    new_str[1] = 0;
  } else {
    new_str = (char *) malloc(sizeof(char) * (fistr->str_size-i+1));
    memcpy(new_str, fistr->str + i, fistr->str_size-i+1);
  }
  fistr->str = new_str;
  free(tmp_fistr_str);

  if (fistr->str_size == i) { // when all digits are 0
    fistr->str_size = 1;
    fistr->sign = POSITIVE;
  }
  else fistr->str_size -= i;
}

void add_leading_zeros_to_fistr(Fistr *fistr, size_t num_of_zeros)
{
  char *tmp_fistr_str = fistr->str;
  char *new_str = (char *) malloc(sizeof(char) * (fistr->str_size+1 + num_of_zeros));
  for (size_t i = 0; i < num_of_zeros; ++i) new_str[i] = '0';
  memcpy(new_str + num_of_zeros, fistr->str, fistr->str_size+1);
  fistr->str = new_str;
  free(tmp_fistr_str);
}

size_t get_str_size_of_operands(Fistr *operand_1, Fistr *operand_2)
{
  size_t str_size;
  if (operand_1->str_size == operand_2->str_size) {
    str_size = operand_1->str_size;
  } else if (operand_1->str_size > operand_2->str_size) {
    str_size = operand_1->str_size;
    add_leading_zeros_to_fistr(operand_2, (size_t) (operand_1->str_size - operand_2->str_size));
    operand_2->str_size += (operand_1->str_size - operand_2->str_size);
  } else if (operand_1->str_size < operand_2->str_size) {
    str_size = operand_2->str_size;
    add_leading_zeros_to_fistr(operand_1, (size_t) (operand_2->str_size - operand_1->str_size));
    operand_1->str_size += (operand_2->str_size - operand_1->str_size);
  }
  
  return str_size;
}

Fistr *get_larger_fistr(Fistr *fistr_1, Fistr *fistr_2)
{
  assert(fistr_1->str_size == fistr_2->str_size);
  for (size_t i = 0; i < fistr_1->str_size; ++i) {
    if (fistr_1->str[i] != fistr_2->str[i]) {
      if (fistr_1->str[i] > fistr_2->str[i]) return fistr_1;
      else if (fistr_1->str[i] < fistr_2->str[i]) return fistr_2;
    }
  }
  return fistr_1;
}

Fistr fistr_add(Fistr *operand_1, Fistr *operand_2)
{
  // Now I am assuming that both operand types are integers
  size_t str_size = get_str_size_of_operands(operand_1, operand_2);
  Fistr_Sign sign;
  
  if (operand_1->sign == POSITIVE && operand_2->sign == POSITIVE) sign = POSITIVE;
  else if (operand_1->sign == NEGATIVE && operand_2->sign == NEGATIVE) sign = NEGATIVE;
  
  char *str = (char *) malloc(sizeof(char) * (str_size + 2));
  for (size_t i = 0; i <= str_size; ++i) str[i] = '0';
  str[str_size+1] = 0;

  if ((operand_1->sign == POSITIVE && operand_2->sign == POSITIVE) || (operand_1->sign == NEGATIVE && operand_2->sign == NEGATIVE)) {
    size_t carry = 0;
    for (size_t i = str_size - 1; i < str_size; --i) {
      size_t sum = (operand_1->str[i] - '0') + (operand_2->str[i] - '0') + carry;
      carry = sum / 10;
      str[i+1] = (sum % 10) + '0';
    }
    str[0] = carry + '0';
  } else {
    size_t borrow = 0;
    Fistr *larger_fistr = get_larger_fistr(operand_1, operand_2);
    if (larger_fistr == operand_1) {
      for (size_t i = str_size - 1; i < str_size; --i) {
	size_t difference = (operand_1->str[i] - borrow) - operand_2->str[i];
	if (difference <= 9) borrow = 0;
	else borrow = 1;
	str[i+1] = difference + (10*borrow) + '0';
      }
      sign = operand_1->sign;
    } else {
      for (size_t i = str_size - 1; i < str_size; --i) {
	size_t difference = (operand_2->str[i] - borrow) - operand_1->str[i];
	if (difference <= 9) borrow = 0;
	else borrow = 1;
	str[i+1] = difference + (10*borrow) + '0';
      }
      sign = operand_2->sign;
    }
  }
  
  Fistr total = {.str = str, .str_size = str_size+1, .type = INTEGER, .sign = sign};
  remove_leading_zeros_from_fistr(&total);
  return total;
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
