#ifndef PRIME_GENERATOR_H
#define PRIME_GENERATOR_H

#include <algorithm>
#include <cmath>
#include <vector>
#include <limits>

template<typename integer>
class prime_generator {
public:
    explicit prime_generator(integer initial_limit = 100, integer increment = 100000);
    integer next_prime();
    integer count() const {
        return count_;
    }
private:
    void find_primes(integer);
    integer count_ = 0;
    integer limit_;
    integer index_ = 0;
    integer increment_;
    std::vector<integer> primes_;
    std::vector<bool> sieve_;
    integer sieve_limit_ = 0;
};

template<typename integer>
integer next_odd_number(integer n) {
    return n % 2 == 0 ? n + 1 : n;
}

template<typename integer>
prime_generator<integer>::prime_generator(integer initial_limit, integer increment)
    : limit_(next_odd_number(initial_limit)), increment_(increment) {
    primes_.push_back(2);
    find_primes(3);
}

template<typename integer>
integer prime_generator<integer>::next_prime() {
    if (index_ == primes_.size()) {
        if (std::numeric_limits<integer>::max() - increment_ < limit_)
            return 0;
        int start = limit_ + 2;
        limit_ = next_odd_number(limit_ + increment_);
        primes_.clear();
        find_primes(start);
    }
    ++count_;
    return primes_[index_++];
}

template<typename integer>
integer isqrt(integer n) {
    return next_odd_number(static_cast<integer>(std::sqrt(n)));
}

template<typename integer>
void prime_generator<integer>::find_primes(integer start) {
    index_ = 0;
    integer new_limit = isqrt(limit_);
    sieve_.resize(new_limit/2);
    for (integer p = 3; p * p <= new_limit; p += 2) {
        if (sieve_[p/2 - 1])
            continue;
        integer q = p * std::max(p, next_odd_number((sieve_limit_ + p - 1)/p));
        for (; q <= new_limit; q += 2*p)
            sieve_[q/2 - 1] = true;
    }
    sieve_limit_ = new_limit;
    size_t count = (limit_ - start)/2 + 1;
    std::vector<bool> composite(count, false);
    for (integer p = 3; p <= new_limit; p += 2) {
        if (sieve_[p/2 - 1])
            continue;
        integer q = p * std::max(p, next_odd_number((start + p - 1)/p)) - start;
        q /= 2;
        for (; q < count; q += p)
            composite[q] = true;
    }
    for (integer p = 0; p < count; ++p) {
        if (!composite[p])
            primes_.push_back(p * 2 + start);
    }
}

#endif // PRIME_GENERATOR_H
