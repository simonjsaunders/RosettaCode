#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

std::vector<bool> prime_sieve(int limit) {
    std::vector<bool> sieve(limit, true);
    if (limit > 0)
        sieve[0] = false;
    if (limit > 1)
        sieve[1] = false;
    for (int i = 4; i < limit; i += 2)
        sieve[i] = false;
    for (int p = 3, sq = 9; sq < limit; p += 2) {
        if (sieve[p]) {
            for (int q = sq; q < limit; q += p << 1)
                sieve[q] = false;
        }
        sq += (p + 1) << 2;
    }
    return sieve;
}

class digit_set {
public:
    explicit digit_set(int n) {
        for (; n > 0; n /= 10)
            ++count_[n % 10];
    }

    bool operator==(const digit_set& other) const {
        return std::equal(count_, count_ + 10, other.count_);
    }

private:
    int count_[10] = {};
};

int main() {
    const int limit = 100000000;
    std::vector<bool> sieve = prime_sieve(limit);
    int count = 0, count1 = 0, count2 = 0;
    std::cout << "First 30 Ormiston pairs:\n";
    for (int p1 = 0, p2 = 0; p2 < limit; ++p2) {
        if (!sieve[p2])
            continue;
        if (digit_set(p2) == digit_set(p1)) {
            if (count1 == 0 && p2 > 1000000)
                count1 = count;
            if (count2 == 0 && p2 > 10000000)
                count2 = count;
            ++count;
            if (count <= 30)
                std::cout << '(' << std::setw(5) << p1 << ", " << std::setw(5)
                          << p2 << ')' << (count % 3 == 0 ? '\n' : ' ');
        }
        p1 = p2;
    }
    std::cout << "\nNumber of Ormiston pairs < 1,000,000: " << count1 << '\n';
    std::cout << "Number of Ormiston pairs < 10,000,000: " << count2 << '\n';
    std::cout << "Number of Ormiston pairs < 100,000,000: " << count << '\n';
}
