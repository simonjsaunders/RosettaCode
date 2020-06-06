#ifndef PRIME_SIEVE_H
#define PRIME_SIEVE_H

#include "bit_array.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct prime_sieve_tag {
    uint32_t limit;
    bit_array not_prime;
} prime_sieve;

bool prime_sieve_create(prime_sieve* s, uint32_t limit);
void prime_sieve_destroy(prime_sieve* s);
bool is_prime(const prime_sieve* s, uint32_t n);

#endif
