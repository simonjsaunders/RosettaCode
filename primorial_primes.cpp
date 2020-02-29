#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <gmpxx.h>

typedef mpz_class integer;

void find_primes(std::vector<bool>& isprime, size_t limit)
{
    isprime.assign(limit, true);
    isprime[0] = isprime[1] = false;
    for (size_t p = 2; p * p < limit; ++p)
    {
        if (isprime[p])
        {
            for (size_t i = p * p; i < limit; i += p)
                isprime[i] = false;
        }
    }
}

bool is_prime(const integer& n)
{
    return mpz_probab_prime_p(n.get_mpz_t(), 25);
}

int main()
{
    const size_t max_prime = 4000;
    const size_t max = 20;

    std::vector<bool> isprime;
    find_primes(isprime, max_prime);
    
    integer primorial = 1;
    for (size_t p = 0, count = 0, index = 0; p < max_prime && count < max; ++p)
    {
        if (!isprime[p])
            continue;
        primorial *= p;
        ++index;
        if (is_prime(primorial - 1) || is_prime(primorial + 1))
        {
            if (count > 0)
                std::cout << ' ';
            std::cout << index;
            ++count;
        }
    }
    std::cout << '\n';

    return 0;
}
