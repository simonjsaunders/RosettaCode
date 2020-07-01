#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include "prime_sieve.hpp"

int main() {
    const int limit1 = 1000000;
    const int limit2 = 10000000;
    const int max_print[2] = { 36, 37 };
    const int array_size = limit2 + 100;

    // find the prime numbers up to array_size
    prime_sieve sieve(array_size);

    // write numbers with groups of digits separated according to the system default locale
    std::cout.imbue(std::locale(""));
    std::cout << std::fixed;

    // count and print strong/weak prime numbers
    int count1[2] = { 0 };
    int count2[2] = { 0 };
    std::ostringstream out[2];
    const char* strength[2] = { "strong", "weak" };
    int p1 = 2, p2 = 3;
    for (int p3 = 5; p2 < limit2; ++p3) {
        if (!sieve.is_prime(p3))
            continue;
        int diff = p1 + p3 - 2 * p2;
        int index = diff < 0 ? 0 : (diff > 0 ? 1 : -1);
        if (index != -1) {
            ++count2[index];
            if (p2 < limit1)
                ++count1[index];
            if (count2[index] <= max_print[index]) {
                if (count2[index] > 1)
                    out[index] << ' ';
                out[index] << p2;
            }
        }
        p1 = p2;
        p2 = p3;
    }
    for (int i = 0; i < 2; ++i) {
        std::cout << "First " << max_print[i] << " " << strength[i] << " primes: " << out[i].str() << '\n';
        std::cout << "Number of " << strength[i] << " primes below " << limit1 << ": " << count1[i] << '\n';
        std::cout << "Number of " << strength[i] << " primes below " << limit2 << ": " << count2[i] << '\n';
    }
    return 0;
}
