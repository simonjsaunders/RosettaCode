#include <assert.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include "prime_sieve.h"

// return number of decimal digits
uint32_t count_digits(uint32_t n) {
    uint32_t digits = 0;
    for (; n > 0; ++digits)
        n /= 10;
    return digits;
}

// return the number with one digit replaced
uint32_t change_digit(uint32_t n, uint32_t index, uint32_t new_digit) {
    uint32_t p = 1;
    uint32_t changed = 0;
    for (; index > 0; p *= 10, n /= 10, --index)
        changed += p * (n % 10);
    changed += (10 * (n/10) + new_digit) * p;
    return changed;
}

// returns true if n unprimeable
bool unprimeable(const prime_sieve* s, uint32_t n) {
    if (is_prime(s, n))
        return false;
    uint32_t d = count_digits(n);
    for (uint32_t i = 0; i < d; ++i) {
        for (uint32_t j = 0; j <= 9; ++j) {
            uint32_t m = change_digit(n, i, j);
            if (m != n && is_prime(s, m))
                return false;
        }
    }
    return true;
}

int main() {
    const uint32_t limit = 10000000;
    setlocale(LC_ALL, "");
    prime_sieve s = { 0 };
    if (!prime_sieve_create(&s, limit)) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    printf("First 35 unprimeable numbers:\n");
    uint32_t n = 100;
    uint32_t lowest[10] = { 0 };
    for (uint32_t count = 0, found = 0; n < limit && (found < 10 || count < 600); ++n) {
        if (unprimeable(&s, n)) {
            if (count < 35) {
                if (count != 0)
                    printf(", ");
                printf("%'u", n);
            }
            ++count;
            if (count == 600)
                printf("\n600th unprimeable number: %'u\n", n);
            uint32_t last_digit = n % 10;
            if (lowest[last_digit] == 0) {
                lowest[last_digit] = n;
                ++found;
            }
        }
    }
    prime_sieve_destroy(&s);
    for (uint32_t i = 0; i < 10; ++i)
        printf("Least unprimeable number ending in %u: %'u\n" , i, lowest[i]);
    return 0;
}
