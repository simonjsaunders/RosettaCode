#include <locale.h>
#include <stdint.h>
#include <stdio.h>

// Returns the sum of the digits of n given the
// sum of the digits of n - 1
uint64_t digit_sum(uint64_t n, int sum) {
    ++sum;
    while (n > 0 && n % 10 == 0) {
        sum -= 9;
        n /= 10;
    }
    return sum;
}

int main() {
    setlocale(LC_ALL, "");

    uint64_t previous = 1, gap = 0;
    int niven_index = 0, gap_index = 1, sum = 0;

    printf("Gap index  Gap    Niven index    Niven number\n");
    for (uint64_t niven = 1; gap_index <= 32; ++niven) {
        sum = digit_sum(niven, sum);
        if (niven % sum == 0) {
            if (niven > previous + gap) {
                gap = niven - previous;
                printf("%'9d %'4llu %'14d %'15llu\n", gap_index++,
                       gap, niven_index, previous);
            }
            previous = niven;
            ++niven_index;
        }
    }
    return 0;
}
