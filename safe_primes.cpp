#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <vector>

int main()
{
    const int limit1 = 1000000;
    const int limit2 = 10000000;
    const int max_print[2] = { 35, 40 };

    // find the prime numbers up to limit2
    std::vector<bool> isprime(limit2, true);
    isprime[0] = isprime[1] = false;
    for (int p = 2; p * p < limit2; ++p)
    {
        if (isprime[p])
        {
            for (int i = p * p; i < limit2; i += p)
                isprime[i] = false;
        }
    }

    // write numbers with groups of digits separated according to the system default locale
    std::cout.imbue(std::locale(""));
    std::cout << std::fixed;

    // count and print safe/unsafe prime numbers
    int count1[2] = { 0 };
    int count2[2] = { 0 };
    std::ostringstream out[2];
    const char* safety[2] = { "safe", "unsafe" };
    for (int p = 2; p < limit2; ++p)
    {
        if (!isprime[p])
            continue;
        int safe = isprime[(p-1)/2] ? 0 : 1;
        ++count2[safe];
        if (p < limit1)
            ++count1[safe];
        if (count2[safe] <= max_print[safe])
        {
            if (count2[safe] > 1)
                out[safe] << ' ';
            out[safe] << p;
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        std::cout << "First " << max_print[i] << " " << safety[i] << " primes: " << out[i].str() << '\n';
        std::cout << "Number of " << safety[i] << " primes below " << limit1 << ": " << count1[i] << '\n';
        std::cout << "Number of " << safety[i] << " primes below " << limit2 << ": " << count2[i] << '\n';
    }
    return 0;
}
