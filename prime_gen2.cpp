#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

template <typename integer>
class prime_generator {
public:
    explicit prime_generator(integer initial_limit = 100, integer increment = 10000);
    bool is_prime(integer n);
    integer nth_prime(integer n);
private:
    void find_primes(integer);
    void extend();
    integer limit_;
    integer increment_;
    std::vector<std::pair<integer, integer>> odd_primes_;
    std::vector<bool> is_odd_prime_;
};

// Constructs a prime generator with the given initial limit (which is rounded
// up to the next odd number).
// The maximum prime number found is increased as required, in steps
// determined by the increment parameter.
template <typename integer>
prime_generator<integer>::prime_generator(integer initial_limit, integer increment)
    : limit_(std::max(integer(3), 1 + 2*(initial_limit/2))),
    increment_(std::max(integer(16), 2*((increment + 1)/2))) {
    find_primes(3);
}

template <typename integer>
void prime_generator<integer>::extend() {
    integer start = limit_ + 2;
    limit_ += increment_;
    find_primes(start);
}

template <typename integer>
void prime_generator<integer>::find_primes(integer start) {
    is_odd_prime_.resize(limit_/2, true);
    // Mark multiples of primes already found as not prime
    for (auto& p : odd_primes_) {
        integer prime = p.first;
        integer multiple = p.second;
        integer increment = 2 * prime; // skip even multiples
        if (multiple > limit_ + increment)
            break;
        for (; multiple <= limit_; multiple += increment)
            is_odd_prime_[multiple/2 - 1] = false;
        p.second = multiple;
    }
    // Look for new odd primes <= limit
    for (integer prime = start; prime * prime <= limit_; prime += 2) {
        if (is_odd_prime_[prime/2 - 1]) {
            integer multiple = prime * prime;
            integer increment = 2 * prime;
            for (; multiple <= limit_; multiple += increment)
                is_odd_prime_[multiple/2 - 1] = false;
        }
    }
    // Add new odd primes to the list
    for (integer prime = start; prime <= limit_; prime += 2) {
        if (is_odd_prime_[prime/2 - 1]) {
            odd_primes_.emplace_back(prime, prime * prime);
        }
    }
}

// Return true if n is prime
template <typename integer>
bool prime_generator<integer>::is_prime(integer n) {
    if (n == 2)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;
    while (n > limit_)
        extend();
    return is_odd_prime_[n/2 - 1];
}

// Return the nth prime number
template <typename integer>
integer prime_generator<integer>::nth_prime(integer n) {
    assert(n > 0);
    if (n == 1)
        return 2;
    while (n >= odd_primes_.size() + 2)
        extend();
    return odd_primes_[n - 2].first;
}
    
int main()
{
    using integer = uint64_t;
    prime_generator<integer> pgen(3, 250000);
    std::cout << "First 20 primes:\n";
    for (integer i = 1; i <= 20; ++i)
    {
        if (i > 1)
            std::cout << ", ";
        std::cout << pgen.nth_prime(i);
    }
    std::cout << '\n';
    std::cout << "Primes between 100 and 150:\n";
    for (integer n = 100, i = 0; n <= 150; ++n)
    {
        if (pgen.is_prime(n))
        {
            if (i++ != 0)
                std::cout << ", ";
            std::cout << n;
        }
    }
    std::cout << '\n';
    int count = 0;
    for (integer n = 7700; n <= 8000; ++n)
    {
        if (pgen.is_prime(n))
            ++count;
    }
    std::cout << "Number of primes between 7700 and 8000: " << count << '\n';
    for (integer n = 10000; n <= 100000000; n *= 10)
        std::cout << n << "th prime: " << pgen.nth_prime(n) << std::endl;
    return 0;
}
