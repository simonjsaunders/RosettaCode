#include <iostream>
#include <cstdint>

using integer = uint32_t;

integer next_prime_digit_number(integer n) {
    if (n == 0)
        return 2;
    switch (n  % 10) {
    case 2:
        return n + 1;
    case 3:
    case 5:
        return n + 2;
    default:
        return 2 + next_prime_digit_number(n/10) * 10;
    }
}

bool is_prime(integer n) {
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;
    for (integer p = 5; p * p <= n; p += 4) {
        if (n % p == 0)
            return false;
        p += 2;
        if (n % p == 0)
            return false;
    }
    return true;
}

int main() {
    const integer limit = 10000000;
    integer n = 0, n1 = 0, n2 = 0, n3 = 0;
    std::cout << "First 25 SPDS primes:\n";
    for (int i = 0; n < limit; ) {
        n = next_prime_digit_number(n);
        if (!is_prime(n))
            continue;
        if (i < 25) {
            if (i > 0)
                std::cout << ", ";
            std::cout << n;
        }
        else if (i == 25)
            std::cout << '\n';
        ++i;
        if (i == 100)
            n1 = n;
        else if (i == 1000)
            n2 = n;
        n3 = n;
    }
    std::cout << "Hundredth SPDS prime: " << n1 << '\n';
    std::cout << "Thousandth SPDS prime: " << n2 << '\n';
    std::cout << "Largest SPDS prime less than " << limit << ": " << n3 << '\n';
    return 0;
}
