#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sieve_tag {
    uint32_t limit;
    bool* not_prime;
} sieve;

bool sieve_create(sieve* s, uint32_t limit) {
    bool* not_prime = calloc(limit + 1, sizeof(bool));
    if (not_prime == NULL)
        return false;
    not_prime[0] = not_prime[1] = true;
    for (uint32_t p = 2; p * p <= limit; ++p) {
        if (not_prime[p] == false) {
            for (size_t q = p * p; q <= limit; q += p)
                not_prime[q] = true;
        }
    }
    s->limit = limit;
    s->not_prime = not_prime;
    return true;
}

void sieve_destroy(sieve* s) {
    free(s->not_prime);
    s->not_prime = NULL;
}

bool is_prime(const sieve* s, uint32_t n) {
    if (n > s->limit)
        return false;
    return s->not_prime[n] == false;
}

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
    sieve s = { 0 };
    if (!sieve_create(&s, limit)) {
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
    sieve_destroy(&s);
    printf("Hundredth SPDS prime: %u\n", n1);
    printf("Thousandth SPDS prime: %u\n", n2);
    printf("Largest SPDS prime less than %u: %u\n", limit, n3);
    return 0;
}
