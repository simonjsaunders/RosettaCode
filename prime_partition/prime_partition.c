#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "prime_sieve.h"

bool find_prime_partition(const prime_sieve* s, uint32_t number, uint32_t count,
                          uint32_t min_prime, uint32_t* p) {
    if (count == 1) {
        if (number >= min_prime && is_prime(s, number)) {
            *p = number;
            return true;
        }
        return false;
    }
    for (uint32_t prime = min_prime; prime < number; ++prime) {
        if (!is_prime(s, prime))
            continue;
        if (find_prime_partition(s, number - prime, count - 1,
                                 prime + 1, p + 1)) {
            *p = prime;
            return true;
        }
    }
    return false;
}

void print_prime_partition(const prime_sieve* s, uint32_t number, uint32_t count) {
    assert(count > 0);
    uint32_t* primes = malloc(count * sizeof(uint32_t));
    if (primes == NULL) {
        fprintf(stderr, "Out of memory\n");
        return;
    }
    if (!find_prime_partition(s, number, count, 2, primes)) {
        printf("%u cannot be partitioned into %u primes.\n", number, count);
    } else {
        printf("%u = %u", number, primes[0]);
        for (uint32_t i = 1; i < count; ++i)
            printf(" + %u", primes[i]);
        printf("\n");
    }
    free(primes);
}

int main() {
    const uint32_t limit = 100000;
    prime_sieve s = { 0 };
    if (!prime_sieve_create(&s, limit)) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    print_prime_partition(&s, 99809, 1);
    print_prime_partition(&s, 18, 2);
    print_prime_partition(&s, 19, 3);
    print_prime_partition(&s, 20, 4);
    print_prime_partition(&s, 2017, 24);
    print_prime_partition(&s, 22699, 1);
    print_prime_partition(&s, 22699, 2);
    print_prime_partition(&s, 22699, 3);
    print_prime_partition(&s, 22699, 4);
    print_prime_partition(&s, 40355, 3);
    prime_sieve_destroy(&s);
    return 0;
}
