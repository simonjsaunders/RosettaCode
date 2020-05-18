#include <iostream>
#include "../library/sieve_of_eratosthenes.h"

int main()
{
    const int limit = 1000000;

    // find the prime numbers up to the limit
    sieve_of_eratosthenes sieve(limit + 1);

    int largest_left = 0;
    int largest_right = 0;
    // find largest left truncatable prime
    for (int p = limit; p >= 2; --p)
    {
        if (!sieve.is_prime(p))
            continue;
        bool left_truncatable = true;
        for (int n = 10, q = p; p > n; n *= 10)
        {
            if (!sieve.is_prime(p % n) || q == p % n)
            {
                left_truncatable = false;
                break;
            }
            q = p % n;
        }
        if (left_truncatable)
        {
            largest_left = p;
            break;
        }
    }
    // find largest right truncatable prime
    for (int p = limit; p >= 2; --p)
    {
        if (!sieve.is_prime(p))
            continue;
        bool right_truncatable = true;
        for (int q = p/10; q > 0; q /= 10)
        {
            if (!sieve.is_prime(q))
            {
                right_truncatable = false;
                break;
            }
        }
        if (right_truncatable)
        {
            largest_right = p;
            break;
        }
    }
    // write results to standard output
    std::cout << "Largest left truncatable prime is " << largest_left << '\n';
    std::cout << "Largest right truncatable prime is " << largest_right << '\n';
    return 0;
}

