#include <cstdint>
#include <iostream>
#include <string>
#include "prime_sieve.hpp"

using integer = uint64_t;

bool square_free(const prime_sieve& sieve, integer n) {
    if (n % 4 == 0)
        return false;
    for (integer p = 3; p * p <= n; p += 2) {
        if (sieve.is_prime(p) && n % (p * p) == 0)
            return false;
    }
    return true;
}

void print_square_free_numbers(const prime_sieve& sieve, integer from, integer to) {
    std::cout << "Square-free numbers between " << from
        << " and " << to << ":\n";
    std::string line;
    for (integer i = from; i <= to; ++i) {
        if (square_free(sieve, i)) {
            if (!line.empty())
                line += ' ';
            line += std::to_string(i);
            if (line.size() >= 80) {
                std::cout << line << '\n';
                line.clear();
            }
        }
    }
    if (!line.empty())
        std::cout << line << '\n';
}

void print_square_free_count(const prime_sieve& sieve, integer from, integer to) {
    integer count = 0;
    for (integer i = from; i <= to; ++i) {
        if (square_free(sieve, i))
            ++count;
    }
    std::cout << "Number of square-free numbers between "
        << from << " and " << to << ": " << count << '\n';
}

int main() {
    prime_sieve sieve(1000001);
    print_square_free_numbers(sieve, 1, 145);
    print_square_free_numbers(sieve, 1000000000000LL, 1000000000145LL);
    print_square_free_count(sieve, 1, 100);
    print_square_free_count(sieve, 1, 1000);
    print_square_free_count(sieve, 1, 10000);
    print_square_free_count(sieve, 1, 100000);
    print_square_free_count(sieve, 1, 1000000);
    return 0;
}
