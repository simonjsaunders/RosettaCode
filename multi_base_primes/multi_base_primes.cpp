#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

std::vector<bool> prime_sieve(uint64_t limit) {
    std::vector<bool> sieve(limit, true);
    if (limit > 0)
        sieve[0] = false;
    if (limit > 1)
        sieve[1] = false;
    for (uint64_t i = 4; i < limit; i += 2)
        sieve[i] = false;
    for (uint64_t p = 3;; p += 2) {
        uint64_t q = p * p;
        if (q >= limit)
            break;
        if (sieve[p]) {
            uint64_t inc = 2 * p;
            for (; q < limit; q += inc)
                sieve[q] = false;
        }
    }
    return sieve;
}

template <typename T> void print(std::ostream& out, const std::vector<T>& v) {
    if (!v.empty()) {
        out << '[';
        auto i = v.begin();
        out << *i++;
        for (; i != v.end(); ++i)
            out << ", " << *i;
        out << ']';
    }
}

std::string to_string(const std::vector<unsigned int>& v) {
    static constexpr char digits[] =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string str;
    for (auto i : v)
        str += digits[i];
    return str;
}

bool increment(std::vector<unsigned int>& digits, unsigned int max_base) {
    auto i = digits.rbegin();
    for (; i != digits.rend() && *i + 1 == max_base; ++i)
        *i = 0;
    if (i == digits.rend())
        return false;
    ++*i;
    return true;
}

void multi_base_primes(unsigned int max_base, unsigned int max_length) {
    auto time1 = std::chrono::high_resolution_clock::now();
    auto sieve =
        prime_sieve(static_cast<uint64_t>(std::pow(max_base, max_length)));
    auto time2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration1(time2 - time1);
    std::cout << "sieve time: " << duration1.count() << "ms\n";
    for (unsigned int length = 1; length <= max_length; ++length) {
        std::cout << length
                  << "-character strings which are prime in most bases: ";
        unsigned int most_bases = 0;
        std::vector<
            std::pair<std::vector<unsigned int>, std::vector<unsigned int>>>
            max_strings;
        std::vector<unsigned int> digits(length, 0);
        digits[0] = 1;
        std::vector<unsigned int> bases;
        do {
            auto max = std::max_element(digits.begin(), digits.end());
            unsigned int min_base = 2;
            if (max != digits.end())
                min_base = std::max(min_base, *max + 1);
            if (most_bases > max_base - min_base + 1)
                continue;
            bases.clear();
            for (unsigned int b = min_base; b <= max_base; ++b) {
                if (max_base - b + 1 + bases.size() < most_bases)
                    break;
                uint64_t n = 0;
                for (auto d : digits)
                    n = n * b + d;
                if (sieve[n])
                    bases.push_back(b);
            }
            if (bases.size() > most_bases) {
                most_bases = bases.size();
                max_strings.clear();
            }
            if (bases.size() == most_bases)
                max_strings.emplace_back(digits, bases);
        } while (increment(digits, max_base));
        std::cout << most_bases << '\n';
        for (const auto& m : max_strings) {
            std::cout << to_string(m.first) << " -> ";
            print(std::cout, m.second);
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    auto time3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration2(time3 - time2);
    std::cout << "remainder: " << duration2.count() << "ms\n";
}

int main(int argc, char** argv) {
    unsigned int max_base = 36;
    unsigned int max_length = 4;
    for (int arg = 1; arg + 1 < argc; ++arg) {
        if (strcmp(argv[arg], "-max_base") == 0)
            max_base = strtoul(argv[++arg], nullptr, 10);
        else if (strcmp(argv[arg], "-max_length") == 0)
            max_length = strtoul(argv[++arg], nullptr, 10);
    }
    if (max_base > 62) {
        std::cerr << "Max base cannot be greater than 62.\n";
        return EXIT_FAILURE;
    }
    if (max_base < 2) {
        std::cerr << "Max base cannot be less than 2.\n";
        return EXIT_FAILURE;
    }
    multi_base_primes(max_base, max_length);
    return EXIT_SUCCESS;
}
