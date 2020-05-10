#include <cstdint>
#include <iomanip>
#include <iostream>

// Returns the sum of the digits of n given the
// sum of the digits of n - 1
uint64_t digit_sum(uint64_t n, int sum)
{
    ++sum;
    while (n > 0 && n % 10 == 0)
    {
        sum -= 9;
        n /= 10;
    }
    return sum;
}

int main()
{
    // Print numbers with commas
    std::cout.imbue(std::locale(""));

    uint64_t previous = 1, gap = 0;
    int niven_index = 0, gap_index = 1, sum = 0;

    std::cout << "Gap index  Gap    Niven index    Niven number\n";
    for (uint64_t niven = 1; gap_index <= 32; ++niven)
    {
        sum = digit_sum(niven, sum);
        if (niven % sum == 0)
        {
            if (niven > previous + gap)
            {
                gap = niven - previous;
                std::cout << std::setw(9) << gap_index++
                    << std::setw(5) << gap
                    << std::setw(15) << niven_index
                    << std::setw(16) << previous << '\n';
            }
            previous = niven;
            ++niven_index;
        }
    }
    return 0;
}
