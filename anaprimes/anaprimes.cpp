#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include <primesieve.hpp>

class digit_set {
public:
    digit_set() {}
    explicit digit_set(uint64_t n) {
        for (; n > 0; n /= 10)
            ++count_[n % 10];
    }
    bool operator==(const digit_set& other) const {
        return std::equal(count_, count_ + 10, other.count_);
    }
    bool operator<(const digit_set& other) const {
        return std::lexicographical_compare(other.count_, other.count_ + 10,
                                            count_, count_ + 10);
    }

private:
    int count_[10] = {};
};

int main() {
    std::cout.imbue(std::locale(""));
    primesieve::iterator pi;
    using map_type = std::map<digit_set, std::vector<uint64_t>>;
    map_type anaprimes;
    for (uint64_t limit = 1000; limit <= 10000000000;) {
        uint64_t prime = pi.next_prime();
        if (prime > limit) {
            size_t max_length = 0;
            std::vector<map_type::iterator> groups;
            for (auto i = anaprimes.begin(); i != anaprimes.end(); ++i) {
                if (i->second.size() > max_length) {
                    groups.clear();
                    max_length = i->second.size();
                }
                if (max_length == i->second.size())
                    groups.push_back(i);
            }
            std::cout << "Largest group(s) of anaprimes before " << limit
                      << ": " << max_length << " members:\n";
            for (auto i : groups) {
                std::cout << "  First: " << i->second.front()
                          << "  Last: " << i->second.back() << '\n';
            }
            std::cout << '\n';
            anaprimes.clear();
            limit *= 10;
        }
        anaprimes[digit_set(prime)].push_back(prime);
    }
}
