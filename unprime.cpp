#include <iostream>
#include <vector>
#include <cstdint>

typedef uint32_t integer;

// n is prime if isprime[n] == true
std::vector<bool> isprime;

// populate isprime up to limit
void find_primes(integer limit)
{
    isprime.assign(limit, true);
    isprime[0] = isprime[1] = false;
    for (integer p = 2; p * p < limit; ++p)
    {
        if (isprime[p])
        {
            for (integer i = p * p; i < limit; i += p)
                isprime[i] = false;
        }
    }
}

// return number of decimal digits
int count_digits(integer n)
{
    int digits = 0;
    for (; n > 0; ++digits)
        n /= 10;
    return digits;
}

// return the number with one digit replaced
integer change_digit(integer n, int index, int new_digit)
{
    integer p = 1;
    integer changed = 0;
    for (; index > 0; p *= 10, n /= 10, --index)
        changed += p * (n % 10);
    changed += (10 * (n/10) + new_digit) * p;
    return changed;
}

// returns true if n unprimeable
bool unprimeable(integer n)
{
    if (isprime[n])
        return false;
    int d = count_digits(n);
    for (int i = 0; i < d; ++i)
    {
        for (int j = 0; j <= 9; ++j)
        {
            integer m = change_digit(n, i, j);
            if (m != n && isprime[m])
                return false;
        }
    }
    return true;
}

int main()
{
    const integer limit = 10000000;
    find_primes(limit);

    // print numbers with commas
    std::cout.imbue(std::locale(""));
    std::cout << std::fixed;

    std::cout << "First 35 unprimeable numbers:\n";
    integer n = 100;
    integer lowest[10] = { 0 };
    for (int count = 0, found = 0; n < limit && (found < 10 || count < 600); ++n)
    {
        if (unprimeable(n))
        {
            if (count < 35)
            {
                if (count != 0)
                    std::cout << ", ";
                std::cout << n;
            }
            ++count;
            if (count == 600)
                std::cout << "\n600th unprimeable number: " << n << '\n';
            int last_digit = n % 10;
            if (lowest[last_digit] == 0)
            {
                lowest[last_digit] = n;
                ++found;
            }
        }
    }
    for (int i = 0; i < 10; ++i)
        std::cout << "Least unprimeable number ending in " << i << ": " << lowest[i] << '\n';
    return 0;
}
