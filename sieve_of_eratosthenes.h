#ifndef SIEVE_OF_ERATOSTHENES_H
#define SIEVE_OF_ERATOSTHENES_H

#include <vector>

class sieve_of_eratosthenes
{
public:
    explicit sieve_of_eratosthenes(size_t);
    bool is_prime(size_t) const;
private:
    std::vector<bool> is_prime_;
};

inline bool sieve_of_eratosthenes::is_prime(size_t n) const
{
    return is_prime_[n];
}

inline sieve_of_eratosthenes::sieve_of_eratosthenes(size_t max)
    : is_prime_(max, true)
{
    is_prime_[0] = is_prime_[1] = false;
    for (size_t p = 2; p * p < max; ++p)
    {
        if (is_prime_[p])
        {
            for (size_t q = p * p; q < max; q +=p)
                is_prime_[q] = false;
        }
    }
}

#endif
