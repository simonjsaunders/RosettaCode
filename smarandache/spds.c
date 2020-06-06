#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "prime_sieve.h"

uint32_t next_prime_digit_number(uint32_t n) {
    if (n == 0)
        return 2;
    switch (n  % 10) {
    case 2:
        return n + 1;
    case 3:
    case 5:
        return n + 2;
    default:
        return 2 + next_prime_digit_number(n/10) * 10;
    }
}

int main() {
    const uint32_t limit = 10000000;
    prime_sieve s = { 0 };
    if (!prime_sieve_create(&s, limit)) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    uint32_t n = 0, n1 = 0, n2 = 0, n3 = 0;
    printf("First 25 SPDS primes:\n");
    for (int i = 0; ; ) {
        n = next_prime_digit_number(n);
        if (n >= limit)
            break;
        if (is_prime(&s, n)) {
            if (i < 25) {
                if (i > 0)
                    printf(", ");
                printf("%u", n);
            }
            else if (i == 25)
                printf("\n");
            ++i;
            if (i == 100)
                n1 = n;
            else if (i == 1000)
                n2 = n;
            n3 = n;
        }
    }
    prime_sieve_destroy(&s);
    printf("Hundredth SPDS prime: %u\n", n1);
    printf("Thousandth SPDS prime: %u\n", n2);
    printf("Largest SPDS prime less than %u: %u\n", limit, n3);
    return 0;
}
