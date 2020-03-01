#include <iostream>
#include <cstdint>
#include <vector>

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

class sieve_of_eratosthenes
{
public:
    explicit sieve_of_eratosthenes(size_t max) : is_prime_(max, true)
    {
        is_prime_[0] = is_prime_[1] = false;
        for (integer p = 2; p * p < max; ++p)
        {
            if (is_prime_[p])
            {
                for (integer q = p * p; q < max; q +=p)
                    is_prime_[q] = false;
            }
        }
    }
    bool is_prime(integer n) const
    {
        return is_prime_[n];
    }
private:
    std::vector<bool> is_prime_;
};

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
