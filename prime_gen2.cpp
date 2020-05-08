#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

class prime_generator {
public:
    explicit prime_generator(size_t initial_limit = 100, size_t increment = 10000);
    bool is_prime(size_t n);
    size_t nth_prime(size_t n);
private:
    void find_primes(size_t);
    void extend();
    size_t limit_;
    size_t increment_;
    std::vector<std::pair<size_t, size_t>> odd_primes_;
    std::vector<bool> is_odd_prime_;
};

// Constructs a prime generator with the given initial limit (which is rounded
// up to the next odd number).
// The maximum prime number found is increased as required, in steps
// determined by the increment parameter.
prime_generator::prime_generator(size_t initial_limit, size_t increment)
    : limit_(std::max(size_t(3), 1 + 2*(initial_limit/2))),
    increment_(std::max(size_t(16), increment)) {
    find_primes(3);
}

void prime_generator::extend() {
    size_t start = limit_ + 2;
    limit_ += increment_;
    find_primes(start);
}

void prime_generator::find_primes(size_t start) {
    is_odd_prime_.resize(limit_/2, true);
    // Mark multiples of primes already found as not prime
    for (auto& p : odd_primes_) {
        size_t prime = p.first;
        size_t multiple = p.second;
        size_t increment = 2 * prime; // skip even multiples
        if (multiple > limit_ + increment)
            break;
        for (; multiple <= limit_; multiple += increment)
            is_odd_prime_[multiple/2 - 1] = false;
        p.second = multiple;
    }
    // Look for new odd primes <= limit
    for (size_t prime = start; prime * prime <= limit_; prime += 2) {
        if (is_odd_prime_[prime/2 - 1]) {
            size_t multiple = prime * prime;
            size_t increment = 2 * prime;
            for (; multiple <= limit_; multiple += increment)
                is_odd_prime_[multiple/2 - 1] = false;
        }
    }
    // Add new odd primes to the list
    for (size_t prime = start; prime <= limit_; prime += 2) {
        if (is_odd_prime_[prime/2 - 1]) {
            odd_primes_.emplace_back(prime, prime * prime);
        }
    }
}

// Return true if n is prime
bool prime_generator::is_prime(size_t n) {
    if (n == 2)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;
    while (n > limit_)
        extend();
    return is_odd_prime_[n/2 - 1];
}

// Return the nth prime number
size_t prime_generator::nth_prime(size_t n) {
    assert(n > 0);
    if (n == 1)
        return 2;
    while (n >= odd_primes_.size() + 2)
        extend();
    return odd_primes_[n - 2].first;
}
    
int main()
{
    prime_generator pgen(3, 250000);
    std::cout << "First 20 primes:\n";
    for (size_t i = 1; i <= 20; ++i)
    {
        if (i > 1)
            std::cout << ", ";
        std::cout << pgen.nth_prime(i);
    }
    std::cout << '\n';
    std::cout << "Primes between 100 and 150:\n";
    for (size_t n = 100, i = 0; n <= 150; ++n)
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
    for (size_t n = 7700; n <= 8000; ++n)
    {
        if (pgen.is_prime(n))
            ++count;
    }
    std::cout << "Number of primes between 7700 and 8000: " << count << '\n';
    for (size_t n = 10000; n <= 100000000; n *= 10)
        std::cout << n << "th prime: " << pgen.nth_prime(n) << '\n';
    return 0;
}
