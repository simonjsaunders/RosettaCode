#include <iomanip>
#include <iostream>
#include <vector>

#include <gmpxx.h>

bool is_curzon(int n, int k) {
    mpz_class p;
    mpz_ui_pow_ui(p.get_mpz_t(), k, n);
    return (p + 1) % (k * n + 1) == 0;
}

int main() {
    for (int k = 2; k <= 10; k += 2) {
        std::cout << "Curzon numbers with base " << k << ":\n";
        int count = 0, n = 1;
        for (; count < 50; ++n) {
            if (is_curzon(n, k)) {
                std::cout << std::setw(4) << n
                          << (++count % 10 == 0 ? '\n' : ' ');
            }
        }
        for (;;) {
            if (is_curzon(n, k))
                ++count;
            if (count == 1000)
                break;
            ++n;
        }
        std::cout << "1000th Curzon number with base " << k << ": " << n
                  << "\n\n";
    }
    return 0;
}
