#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

#include <primesieve.hpp>

class digit_set {
public:
    digit_set() {}
    explicit digit_set(uint64_t n) {
        for (; n > 0; n /= 10)
            ++count_[n % 10];
    }

    bool operator==(const digit_set& other) const {
        return std::equal(count_, count_ + 10, other.count_);
    }

private:
    int count_[10] = {};
};

class ormiston_pair_generator {
public:
    ormiston_pair_generator() { prime_ = pi_.next_prime(); }
    std::pair<uint64_t, uint64_t> next_pair() {
        for (;;) {
            uint64_t prime = prime_;
            auto digits = digits_;
            prime_ = pi_.next_prime();
            digits_ = digit_set(prime_);
            if (digits_ == digits)
                return std::make_pair(prime, prime_);
        }
    }

private:
    primesieve::iterator pi_;
    uint64_t prime_;
    digit_set digits_;
};

int main() {
    ormiston_pair_generator generator;
    int count = 0;
    std::cout << "First 30 Ormiston pairs:\n";
    for (; count < 30; ++count) {
        auto [p1, p2] = generator.next_pair();
        std::cout << '(' << std::setw(5) << p1 << ", " << std::setw(5) << p2
                  << ')' << ((count + 1) % 3 == 0 ? '\n' : ' ');
    }
    std::cout << '\n';
    for (uint64_t limit = 1000000; limit <= 1000000000; ++count) {
        auto [p1, p2] = generator.next_pair();
        if (p1 > limit) {
            std::cout << "Number of Ormiston pairs < " << limit << ": " << count
                      << '\n';
            limit *= 10;
        }
    }
}
