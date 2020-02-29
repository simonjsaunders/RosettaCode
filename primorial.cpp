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

size_t count_digits(const integer& n)
{
    std::ostringstream out;
    out << n;
    return out.str().length();
}

int main()
{
    const size_t max_prime = 20000000;
    const size_t max_index = 1000000;

    std::vector<bool> isprime;
    find_primes(isprime, max_prime);
    
    integer primorial = 1;
    for (size_t p = 0, index = 0, power = 10; p < max_prime && index <= max_index; ++p)
    {
        if (!isprime[p])
            continue;
        if (index < 10)
            std::cout << "primorial(" << index << ") = " << primorial << '\n';
        else if (index == power)
        {
            std::cout << "primorial(" << index << ") has length "
                << count_digits(primorial) << '\n';
            power *= 10;
        }
        ++index;
        primorial *= p;
    }

    return 0;
}
