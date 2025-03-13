#define FISTR_IMPLEMENTATION
#include "../fistr.h"

Fistr factorial(size_t num)
{
  Fistr a, b, c;

  b = int_as_fistr(1);
  for (size_t i = 1; i <= num; ++i) {
    a = int_as_fistr(i);
    c = b;
    b = fistr_mult(&b, &a);
    free(a.str);
    free(c.str);
  }

  return b;
}

Fistr nCk(size_t n, size_t k)
{
  assert(n >= k);
  Fistr n_fact = factorial(n);
  Fistr k_fact = factorial(k);
  Fistr n_minus_k_fact = factorial(n-k);

  Fistr result = fistr_mult(&k_fact, &n_minus_k_fact);
  result = fistr_div(&n_fact, &result);
  free(n_fact.str);
  free(k_fact.str);
  free(n_minus_k_fact.str);
  return result;
}

int main(void)
{
  // 128635215 byte(s) leaked in 4712932 allocation(s)  == 128.63MB
  // when we first divide n! by k! and then divide the result by (n-k)!

  // 5601760 byte(s) leaked in 204657 allocation(s)  == 5.6MB
  // when we multiply k! and (n-k)! and divide the result by n!

  for (size_t i = 2; i <= 4; ++i) {
    Fistr n_C_k = nCk(40, i);
    print_fistr(&n_C_k);
    free(n_C_k.str);
  }

  return 0;
}
