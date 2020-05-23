#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include "../library/prime_generator.h"

void compute_transitions(size_t limit) {
    prime_generator<size_t> pgen(10000, 500000);
    std::map<std::pair<size_t, size_t>, size_t> transitions;
    for (size_t count = 0, prev = 0; count < limit; ++count) {
        size_t n = pgen.next_prime();
        size_t digit = n % 10;
        if (prev != 0)
            ++transitions[std::make_pair(prev, digit)];
        prev = digit;
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
