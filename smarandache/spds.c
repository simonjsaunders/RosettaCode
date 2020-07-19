#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint32_t next_prime_digit_number(uint32_t n) {
    if (n == 0)
        return 2;
    switch (n % 10) {
    case 2:
        return n + 1;
    case 3:
    case 5:
        return n + 2;
    default:
        return 2 + next_prime_digit_number(n/10) * 10;
    }
}

bool is_prime(uint32_t n) {
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;
    for (uint32_t p = 5; p * p <= n; p += 4) {
        if (n % p == 0)
            return false;
        p += 2;
        if (n % p == 0)
            return false;
    }
    return true;
}

int main() {
    const uint32_t limit = 10000000;
    uint32_t n = 0, n1 = 0, n2 = 0, n3 = 0;
    printf("First 25 SPDS primes:\n");
    for (int i = 0; n < limit; ) {
        n = next_prime_digit_number(n);
        if (!is_prime(n))
            continue;
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
    printf("Hundredth SPDS prime: %u\n", n1);
    printf("Thousandth SPDS prime: %u\n", n2);
    printf("Largest SPDS prime less than %u: %u\n", limit, n3);
    return 0;
}
