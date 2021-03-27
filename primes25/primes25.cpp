#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include <gmpxx.h>

bool is_probably_prime(const mpz_class& n) {
    return mpz_probab_prime_p(n.get_mpz_t(), 25) != 0;
}

bool is_prime(int n) {
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;
    for (int p = 5; p * p <= n; p += 4) {
        if (n % p == 0)
            return false;
        p += 2;
        if (n % p == 0)
            return false;
    }
    return true;
}

int digit_sum(int n) {
    int sum = 0;
    for (; n > 0; n /= 10)
        sum += n % 10;
    return sum;
}

void countAll(const std::string& str, int rem, int& count) {
    if (rem == 0) {
        switch (str.back()) {
        case '1':
        case '3':
        case '7':
        case '9':
            if (is_probably_prime(mpz_class(str)))
                ++count;
            break;
        default:
            break;
        }
    } else {
        for (int i = 1; i <= std::min(9, rem); ++i)
            countAll(str + std::to_string(i), rem - i, count);
    }
}

int main() {
    std::cout.imbue(std::locale(""));
    const int limit = 5000;
    std::cout << "Primes < " << limit << " whose digits sum to 25:\n";
    int count = 0;
    for (int p = 1; p < limit; ++p) {
        if (digit_sum(p) == 25 && is_prime(p)) {
            ++count;
            std::cout << std::setw(6) << p << (count % 10 == 0 ? '\n' : ' ');
        }
    }
    std::cout << '\n';

    count = 0;
    auto start = std::chrono::steady_clock::now();
    countAll("", 25, count);
    auto end = std::chrono::steady_clock::now();
    std::cout << "\nThere are " << count
              << " primes whose digits sum to 25 and include no zeros.\n";
    std::cout << "Time taken: "
              << std::chrono::duration<double>(end - start).count() << "s\n";
    return 0;
}
