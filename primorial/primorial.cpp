#include <gmpxx.h>
#include <primesieve.hpp>

#include <cstdint>
#include <iomanip>
#include <iostream>

size_t digits(const mpz_class& n) { return n.get_str().length(); }

mpz_class primorial(unsigned int n) {
    mpz_class p;
    mpz_primorial_ui(p.get_mpz_t(), n);
    return p;
}

int main() {
    uint64_t index = 0;
    primesieve::iterator pi;
    std::cout << "First 10 primorial numbers:\n";
    for (; index < 10; ++index) {
        uint64_t prime = pi.next_prime();
        std::cout << index << ": " << primorial(prime - 1) << '\n';
    }
    std::cout << "\nLength of primorial number whose index is:\n";
    for (uint64_t power = 10; index <= 1000000; ++index) {
        uint64_t prime = pi.next_prime();
        if (index == power) {
            std::cout << std::setw(7) << index << ": "
                      << digits(primorial(prime - 1)) << '\n';
            power *= 10;
        }
    }
    return 0;
}
