#include <cstdint>
#include <iostream>
#include <sstream>
#include <gmpxx.h>
#include "prime_sieve.hpp"

typedef mpz_class integer;

size_t count_digits(const integer& n) {
    std::ostringstream out;
    out << n;
    return out.str().length();
}

int main() {
    const size_t max_prime = 20000000;
    const size_t max_index = 1000000;
    prime_sieve sieve(max_prime);
    integer primorial = 1;
    for (size_t p = 0, index = 0, power = 10; p < max_prime && index <= max_index; ++p) {
        if (!sieve.is_prime(p))
            continue;
        if (index < 10)
            std::cout << "primorial(" << index << ") = " << primorial << '\n';
        else if (index == power) {
            std::cout << "primorial(" << index << ") has length "
                << count_digits(primorial) << '\n';
            power *= 10;
        }
        ++index;
        primorial *= p;
    }
    return 0;
}
