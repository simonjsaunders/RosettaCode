#include "prime_sieve.h"
#include <assert.h>

bool prime_sieve_create(prime_sieve* s, uint32_t limit) {
    if (!bit_array_create(&s->not_prime, limit/2))
        return false;
    for (uint32_t p = 3; p * p <= limit; p += 2) {
        if (bit_array_get(&s->not_prime, p/2 - 1) == false) {
            uint32_t inc = 2 * p;
            for (uint32_t q = p * p; q <= limit; q += inc)
                bit_array_set(&s->not_prime, q/2 - 1, true);
        }
    }
    s->limit = limit;
    return true;
}

void prime_sieve_destroy(prime_sieve* s) {
    bit_array_destroy(&s->not_prime);
}

bool is_prime(const prime_sieve* s, uint32_t n) {
    assert(n <= s->limit);
    if (n == 2)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;
    return bit_array_get(&s->not_prime, n/2 - 1) == false;
}
