#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct bit_array_tag {
    uint32_t size;
    uint32_t* array;
} bit_array;

bool bit_array_create(bit_array* b, uint32_t size) {
    uint32_t* array = calloc((size + 31)/32, sizeof(uint32_t));
    if (array == NULL)
        return false;
    b->size = size;
    b->array = array;
    return true;
}

void bit_array_destroy(bit_array* b) {
    free(b->array);
    b->array = NULL;
}

void bit_array_set(bit_array* b, uint32_t index, bool value) {
    assert(index < b->size);
    uint32_t* p = &b->array[index >> 5];
    uint32_t bit = 1 << (index & 31);
    if (value)
        *p |= bit;
    else
        *p &= ~bit;
}

bool bit_array_get(const bit_array* b, uint32_t index) {
    assert(index < b->size);
    uint32_t* p = &b->array[index >> 5];
    uint32_t bit = 1 << (index & 31);
    return (*p & bit) != 0;
}

typedef struct sieve_tag {
    uint32_t limit;
    bit_array not_prime;
} sieve;

bool sieve_create(sieve* s, uint32_t limit) {
    if (!bit_array_create(&s->not_prime, limit + 1))
        return false;
    bit_array_set(&s->not_prime, 0, true);
    bit_array_set(&s->not_prime, 1, true);
    for (uint32_t p = 2; p * p <= limit; ++p) {
        if (bit_array_get(&s->not_prime, p) == false) {
            for (uint32_t q = p * p; q <= limit; q += p)
                bit_array_set(&s->not_prime, q, true);
        }
    }
    s->limit = limit;
    return true;
}

void sieve_destroy(sieve* s) {
    bit_array_destroy(&s->not_prime);
}

bool is_prime(const sieve* s, uint32_t n) {
    assert(n <= s->limit);
    return bit_array_get(&s->not_prime, n) == false;
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
