#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include "../library/sieve_of_eratosthenes.h"

// See https://en.wikipedia.org/wiki/Prime_number_theorem#Approximations_for_the_nth_prime_number
size_t upper_bound_for_nth_prime(size_t n) {
    return static_cast<size_t>(n * (std::log(n) + std::log(std::log(n))));
}

void compute_transitions(size_t limit) {
    sieve_of_eratosthenes sieve(upper_bound_for_nth_prime(limit));
    size_t prev = 2;
    size_t count = 0;
    std::map<std::pair<size_t, size_t>, size_t> transitions;
    for (size_t n = 3; count < limit; n += 2) {
        if (sieve.is_prime(n)) {
            size_t digit = n % 10;
            ++transitions[std::make_pair(prev, digit)];
            ++count;
            prev = digit;
        }
    }
    std::cout << "First " << limit << " prime numbers:\n";
    for (auto&& pair : transitions) {
        double freq = (100.0 * pair.second)/limit;
        std::cout << pair.first.first << " -> " << pair.first.second
            << ": count = " << std::setw(7) << pair.second
            << ", frequency = " << std::setprecision(2)
            << std::fixed << freq << " %\n";
    }
}

int main(int argc, const char * argv[]) {
    compute_transitions(1000000);
    compute_transitions(100000000);
    return 0;
}
