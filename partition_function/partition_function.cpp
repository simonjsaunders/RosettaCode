#include <chrono>
#include <iostream>
#include <vector>
#include <gmpxx.h>

using big_int = mpz_class;

big_int partitions(int n) {
    std::vector<big_int> p(n + 1);
    p[0] = 1;
    for (int i = 1; i <= n; ++i) {
        big_int result = 0;
        for (int k = 1, sign = 1;; ++k, sign = -sign) {
            int m = (k * (3*k - 1))/2;
            if (m > i)
                break;
            if (sign == 1)
                result += p[i - m];
            else
                result -= p[i - m];
            m = (k * (3*k + 1))/2;
            if (m > i)
                break;
            if (sign == 1)
                result += p[i - m];
            else
                result -= p[i - m];
        }
        p[i] = result;
    }
    return p[n];
}

int main() {
    auto start = std::chrono::steady_clock::now();
    auto result = partitions(6666);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> ms(end - start);
    std::cout << result << '\n';
    std::cout << "elapsed time: " << ms.count() << " milliseconds\n";
}
