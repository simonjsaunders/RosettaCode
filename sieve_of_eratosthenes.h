#ifndef SIEVE_OF_ERATOSTHENES_H
#define SIEVE_OF_ERATOSTHENES_H

#include <algorithm>
#include <vector>

class sieve_of_eratosthenes {
public:
    explicit sieve_of_eratosthenes(size_t);
    bool is_prime(size_t) const;
private:
    std::vector<bool> odd_prime_;
};

inline bool sieve_of_eratosthenes::is_prime(size_t n) const {
    if (n == 2)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;
    return odd_prime_[n/2 - 1];
}

inline sieve_of_eratosthenes::sieve_of_eratosthenes(size_t limit) {
    limit = std::max(size_t(3), 1 + 2*(limit/2));
    odd_prime_.resize(limit/2, true);
    for (size_t p = 3; p * p <= limit; p += 2) {
        if (odd_prime_[p/2 - 1]) {
            size_t inc = 2 * p;
            for (size_t q = p * p; q <= limit; q += inc)
                odd_prime_[q/2 - 1] = false;
        }
    }
}

#endif
