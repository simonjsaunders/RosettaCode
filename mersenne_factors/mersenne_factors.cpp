#include <iostream>
#include <cstdint>
#include <vector>
#include "../library/sieve_of_eratosthenes.h"

typedef uint64_t integer;

integer bit_count(integer n)
{
    integer count = 0;
    while (n > 0)
    {
        n >>= 1;
        ++count;
    }
    return count;
}

integer mod_pow(integer p, integer n)
{
    integer square = 1;
    integer bits = bit_count(p);
    while (bits > 0)
    {
        square = square * square;
        if ((p & (1 << --bits)) != 0)
            square <<= 1;
        square %= n;
    }
    return square;
}

const integer limit = 100000U;

integer find_mersenne_factor(const sieve_of_eratosthenes& sieve, integer p)
{
    integer k = 0;
    integer q = 1;
    for (;;)
    {
        ++k;
        q = 2 * k * p + 1;
        if (q >= limit)
            break;
        if (q % 8 == 1 || q % 8 == 7)
        {
            if (mod_pow(p, q) == 1 && sieve.is_prime(q))
                return q;
        }
    }
    return 0;
}

int main()
{
    sieve_of_eratosthenes sieve(limit);
    std::cout << find_mersenne_factor(sieve, 929) << '\n';
    return 0;
}
