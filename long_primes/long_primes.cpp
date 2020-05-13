#include <cstdint>
#include <iostream>
#include "../library/sieve_of_eratosthenes.h"

// References:
// https://en.wikipedia.org/wiki/Full_reptend_prime
// https://en.wikipedia.org/wiki/Primitive_root_modulo_n#Finding_primitive_roots

uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    if (mod == 1)
        return 0;
    uint64_t result = 1;
    base %= mod;
    for (; exp > 0; exp >>= 1) {
        if ((exp & 1) == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
    }
    return result;
}

bool is_long_prime(const sieve_of_eratosthenes& sieve, uint64_t prime) {
    if (10 % prime == 0)
        return false;
    uint64_t n = prime - 1, m = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (sieve.is_prime(p) && m % p == 0) {
            if (modpow(10, n/p, prime) == 1)
                return false;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m == 1)
        return true;
    return modpow(10, n/m, prime) != 1;
}

void long_primes(uint64_t stage, uint64_t limit) {
    sieve_of_eratosthenes sieve(limit);
    uint64_t count = 0;
    uint64_t stage2 = stage;
    for (uint64_t p = 3; p < limit; p += 2) {
        if (sieve.is_prime(p) && is_long_prime(sieve, p)) {
            if (p < 500)
                std::cout << p << ' ';
            if (p > stage2) {
                std::cout << "\nNumber of long primes up to " << stage2 << ": " << count;
                stage2 *= 2;
            }
            ++count;
        }
    }
    std::cout << "\nNumber of long primes up to " << stage2 << ": " << count << '\n';
}

int main() {
    long_primes(500, 8192000);
    return 0;
}
