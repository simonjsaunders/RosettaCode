#include <array>
#include <iostream>
#include <vector>
#include "../library/circular_buffer.h"
#include "../library/sieve_of_eratosthenes.h"

int main() {
    using std::cout;
    using std::vector;

    const int max = 1000035;
    const int max_group_size = 5;
    const int diff = 6;
    const int array_size = max + diff;
    const int max_groups = 5;
    const int max_unsexy = 10;

    // Use Sieve of Eratosthenes to find prime numbers up to max
    sieve_of_eratosthenes sieve(array_size);

    std::array<int, max_group_size> group_count{0};
    vector<circular_buffer<vector<int>>> groups;
    for (size_t i = 0; i < max_group_size; ++i)
        groups.emplace_back(max_groups);
    int unsexy_count = 0;
    circular_buffer<int> unsexy_primes(max_unsexy);

    for (int p = 2; p < max; ) {
        if (!sieve.is_prime(p + diff) && (p - diff < 2 || !sieve.is_prime(p - diff))) {
            // if p + diff and p - diff aren't prime then p can't be sexy
            ++unsexy_count;
            unsexy_primes.push_back(p);
        } else {
            // find the groups of sexy primes that begin with p
            int group_size = 1;
            vector<int> group;
            group.push_back(p);
            for (int i = 1; i < max_group_size; ++i) {
                int next_p = p + i * diff;
                if (next_p >= max || !sieve.is_prime(next_p))
                    break;
                ++group_size;
                group.push_back(next_p);
                groups[group_size - 1].push_back(group);
            }
            for (int i = 1; i < group_size; ++i)
                ++group_count[i];
        }
        // skip to next prime number
        ++p;
        while (p < max && !sieve.is_prime(p))
            ++p;
    }

    for (int size = 1; size < max_group_size; ++size) {
        cout << "number of groups of size " << size + 1 << " is " << group_count[size] << '\n';
        cout << "last " << groups[size].size() << " groups of size " << size + 1 << ":";
        for (const vector<int>& group : groups[size]) {
            cout << " (";
            for (size_t i = 0; i < group.size(); ++i) {
                if (i > 0)
                    cout << ' ';
                cout << group[i];
            }
            cout << ")";
        }
        cout << "\n\n";
    }
    cout << "number of unsexy primes is " << unsexy_count << '\n';
    cout << "last " << unsexy_primes.size() << " unsexy primes:";
    for (int prime : unsexy_primes)
        cout << ' ' << prime;
    cout << '\n';
    return 0;
}
