#include <cstdint>
#include <iostream>
#include "prime_sieve.hpp"

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

bool is_long_prime(const prime_sieve& sieve, uint64_t prime) {
    if (!sieve.is_prime(prime))
        return false;
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

void long_primes(uint64_t limit1, uint64_t limit2) {
    prime_sieve sieve(limit2);
    uint64_t count = 0;
    uint64_t limit = limit1;
    for (uint64_t p = 3; p < limit2; p += 2) {
        if (is_long_prime(sieve, p)) {
            if (p < limit1)
                std::cout << p << ' ';
            if (p > limit) {
                std::cout << "\nNumber of long primes up to " << limit << ": " << count;
                limit *= 2;
            }
            ++count;
        }
    }
    std::cout << "\nNumber of long primes up to " << limit << ": " << count << '\n';
}

int main() {
    long_primes(500, 8192000);
    return 0;
}
