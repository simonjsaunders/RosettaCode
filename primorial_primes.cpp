#include <cstdint>
#include <iostream>
#include <sstream>
#include <gmpxx.h>
#include "sieve_of_eratosthenes.h"

typedef mpz_class integer;

bool is_prime(const integer& n)
{
    return mpz_probab_prime_p(n.get_mpz_t(), 25);
}

int main()
{
    const size_t max_prime = 4000;
    const size_t max = 20;

    sieve_of_eratosthenes sieve(max_prime);
    
    integer primorial = 1;
    for (size_t p = 0, count = 0, index = 0; p < max_prime && count < max; ++p)
    {
        if (!sieve.is_prime(p))
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
