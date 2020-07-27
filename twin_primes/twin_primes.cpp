#include <cstdint>
#include <iostream>
#include <primesieve.hpp>

int main() {
    std::cout.imbue(std::locale(""));
    uint64_t limit = 10;
    for (int power = 1; power < 12; ++power, limit *= 10) {
        std::cout << "Number of twin prime pairs less than " << limit
            << " is " << primesieve::count_twins(0, limit) << '\n';
    }
    return 0;
}
