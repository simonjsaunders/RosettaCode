#include <iostream>
#include <vector>
#include "sieve_of_eratosthenes.h"

int main()
{
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

    vector<int> group_count(max_group_size);
    vector<vector<vector<int>>> groups(max_group_size);
    int unsexy_count = 0;
    vector<int> unsexy_primes;

    for (int p = 2; p < max; )
    {
        if (!sieve.is_prime(p + diff) && (p - diff < 2 || !sieve.is_prime(p - diff)))
        {
            // if p + diff and p - diff aren't prime then p can't be sexy
            ++unsexy_count;
            if (unsexy_primes.size() == max_unsexy)
                unsexy_primes.erase(unsexy_primes.begin());
            unsexy_primes.push_back(p);
        }
        else
        {
            // find the groups of sexy primes that begin with p
            int group_size = 1;
            vector<int> group;
            group.push_back(p);
            for (int i = 1; i < max_group_size; ++i)
            {
                int next_p = p + i * diff;
                if (next_p >= max || !sieve.is_prime(next_p))
                    break;
                ++group_size;
                group.push_back(next_p);
                vector<vector<int>>& v = groups[group_size - 1];
                if (v.size() == max_groups)
                    v.erase(v.begin());
                v.push_back(group);
            }
            for (int i = 1; i < group_size; ++i)
                ++group_count[i];
        }
        // skip to next prime number
        ++p;
        while (p < max && !sieve.is_prime(p))
            ++p;
    }

    for (int size = 1; size < max_group_size; ++size)
    {
        cout << "number of groups of size " << size + 1 << " is " << group_count[size] << '\n';
        cout << "last " << groups[size].size() << " groups of size " << size + 1 << ":";
        for (const vector<int>& group : groups[size])
        {
            cout << " (";
            for (size_t i = 0; i < group.size(); ++i)
            {
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
