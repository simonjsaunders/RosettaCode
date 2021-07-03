#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

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

template <typename T>
void print(std::ostream& out, const std::vector<T>& v) {
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
    static constexpr char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string str;
    for (auto i : v)
        str += digits[i];
    return str;
};

class multi_base_primes {
public:
    explicit multi_base_primes(unsigned int depth);
    void run();

private:
    void process(const std::vector<unsigned int>& indices);
    void nested_for(std::vector<unsigned int>& indices, unsigned int level);
    static const unsigned int max_base = 36;
    unsigned int max_depth;
    std::vector<bool> sieve;
    unsigned int most_bases = 0;
    std::vector<std::pair<std::vector<unsigned int>, std::vector<unsigned int>>>
        max_strings;
};

multi_base_primes::multi_base_primes(unsigned int depth)
    : max_depth(depth),
      sieve(prime_sieve(static_cast<uint64_t>(std::pow(max_base, depth)))) {}

void multi_base_primes::run() {
    for (unsigned int depth = 1; depth <= max_depth; ++depth) {
        std::cout << depth
                  << " character strings which are prime in most bases: ";
        max_strings.clear();
        most_bases = 0;
        std::vector<unsigned int> indices(depth, 0);
        nested_for(indices, 0);
        std::cout << most_bases << '\n';
        for (const auto& m : max_strings) {
            std::cout << to_string(m.first) << " -> ";
            print(std::cout, m.second);
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}

void multi_base_primes::process(const std::vector<unsigned int>& indices) {
    std::vector<unsigned int> bases;
    auto max = std::max_element(indices.begin(), indices.end());
    unsigned int min_base = 2;
    if (max != indices.end())
        min_base = std::max(min_base, *max + 1);
    for (unsigned int b = min_base; b <= max_base; ++b) {
        uint64_t n = 0;
        for (auto i : indices)
            n = n * b + i;
        if (sieve[n])
            bases.push_back(b);
    }
    if (bases.size() > most_bases) {
        most_bases = bases.size();
        max_strings.clear();
    }
    if (bases.size() == most_bases)
        max_strings.emplace_back(indices, bases);
}

void multi_base_primes::nested_for(std::vector<unsigned int>& indices,
                                   unsigned int level) {
    if (level == indices.size()) {
        process(indices);
    } else {
        indices[level] = (level == 0) ? 1 : 0;
        while (indices[level] < max_base) {
            nested_for(indices, level + 1);
            ++indices[level];
        }
    }
}

int main() {
    multi_base_primes mbp(6);
    mbp.run();
}
