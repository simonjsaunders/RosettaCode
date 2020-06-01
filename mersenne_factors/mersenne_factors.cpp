#include <iostream>
#include <cstdint>
#include <vector>

typedef uint64_t integer;

integer bit_count(integer n) {
    integer count = 0;
    while (n > 0) {
        n >>= 1;
        ++count;
    }
    return count;
}

integer mod_pow(integer p, integer n) {
    integer square = 1;
    integer bits = bit_count(p);
    while (bits > 0) {
        square = square * square;
        if ((p & (1 << --bits)) != 0)
            square <<= 1;
        square %= n;
    }
    return square;
}

bool is_prime(integer n) {
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (integer p = 3; p * p <= n; p += 2) {
        if (n % p == 0)
            return false;
    }
    return true;
}

integer find_mersenne_factor(integer p) {
    integer k = 0;
    integer q = 1;
    for (;;) {
        ++k;
        q = 2 * k * p + 1;
        if (q % 8 == 1 || q % 8 == 7) {
            if (mod_pow(p, q) == 1 && is_prime(q))
                return q;
        }
    }
    return 0;
}

int main() {
    std::cout << find_mersenne_factor(929) << '\n';
    return 0;
}
