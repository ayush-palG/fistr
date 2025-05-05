#define FISTR_IMPLEMENTATION
#include "fistr.h"

Fistr factorial(size_t num)
{
  Fistr start = int_as_fistr(1);
  for (size_t i = 1; i <= num; ++i) {
    Fistr next = int_as_fistr(i);
    fistr_mul(&start, &next);
    free(next.string.buffer);
  }

  return start;
}

Fistr nCk(size_t n, size_t k)
{
  assert(n >= k);
  Fistr n_fact = factorial(n);
  Fistr k_fact = factorial(k);
  Fistr n_minus_k_fact = factorial(n-k);

  fistr_div(&n_fact, &n_minus_k_fact);
  fistr_div(&n_fact, &k_fact);
  free(k_fact.string.buffer);
  free(n_minus_k_fact.string.buffer);
  
  return n_fact;
}

Fistr nPk(size_t n, size_t k)
{
  assert(n >= k);
  Fistr n_fact = factorial(n);
  Fistr n_minus_k_fact = factorial(n-k);

  fistr_div(&n_fact, &n_minus_k_fact);
  free(n_minus_k_fact.string.buffer);
  
  return n_fact;
}

int main(void)
{
  size_t n = 20;
  for (size_t i = 0; i <= n; ++i) {
    Fistr n_C_k = nCk(n, i);
    fistr_print(n_C_k);
    free(n_C_k.string.buffer);
  }

  return 0;
}
