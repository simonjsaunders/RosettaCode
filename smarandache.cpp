#include <iostream>
#include <cstdint>
#include "sieve_of_eratosthenes.h"

using integer = uint32_t;

integer next_prime_digit_number(integer n) {
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
    const integer limit = 10000000;
    sieve_of_eratosthenes sieve(limit);
    integer n = 0, n1 = 0, n2 = 0, n3 = 0;
    std::cout << "First 25 SPDS primes:\n";
    for (int i = 0; ; ) {
        n = next_prime_digit_number(n);
        if (n >= limit)
            break;
        if (sieve.is_prime(n)) {
            if (i < 25) {
                if (i > 0)
                    std::cout << ", ";
                std::cout << n;
            }
            else if (i == 25)
                std::cout << '\n';
            ++i;
            if (i == 100)
                n1 = n;
            else if (i == 1000)
                n2 = n;
            n3 = n;
        }
    }
    std::cout << "Hundredth SPDS prime: " << n1 << '\n';
    std::cout << "Thousandth SPDS prime: " << n2 << '\n';
    std::cout << "Largest SPDS prime less than " << limit << ": " << n3 << '\n';
    return 0;
}