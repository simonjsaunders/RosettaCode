#include <iostream>
#include <vector>
#include <algorithm>

typedef unsigned long long integer;

// find the prime numbers up to limit
std::vector<integer> find_primes(size_t limit)
{
    std::vector<bool> isprime(limit, true);
    isprime[0] = isprime[1] = false;
    for (size_t p = 2; p * p < limit; ++p)
    {
        if (isprime[p])
        {
            for (size_t i = p * p; i < limit; i += p)
                isprime[i] = false;
        }
    }
    std::vector<integer> primes;
    for (size_t p = 2; p < limit; ++p)
    {
        if (isprime[p])
            primes.push_back(p);
    }
    return primes;
}

// returns all ancestors of n. n is not its own ancestor.
std::vector<integer> get_ancestors(const std::vector<integer>& ancestor, integer n)
{
    std::vector<integer> result;
    for (int a = ancestor[n]; a != 0 && a != n; )
    {
        n = a;
        a = ancestor[n];
        result.push_back(n);
    }
    return result;
}

void print_vector(const std::vector<integer>& vec)
{
    if (vec.empty())
    {
        std::cout << "none\n";
        return;
    }
    auto i = vec.begin();
    std::cout << *i++;
    for (; i != vec.end(); ++i)
        std::cout << ", " << *i;
    std::cout << '\n';
}

int main(int argc, char** argv)
{
    const size_t limit = 100;
    std::vector<integer> primes(find_primes(limit));

    std::vector<integer> ancestor(limit, 0);
    std::vector<std::vector<integer>> descendants(limit);

    for (integer prime : primes)
    {
        descendants[prime].push_back(prime);
        for (size_t i = 0; i + prime < limit; ++i)
        {
            integer s = i + prime;
            for (integer n : descendants[i])
            {
                integer prod = n * prime;
                descendants[s].push_back(prod);
                if (prod < limit)
                    ancestor[prod] = s;
            }
        }
    }

    // print the results
    size_t total_descendants = 0;
    for (integer i = 1; i < limit; ++i)
    {
        std::vector<integer> ancestors(get_ancestors(ancestor, i));
        std::cout << "[" << i << "] Level: " << ancestors.size() << '\n';
        std::cout << "Ancestors: ";
        std::sort(ancestors.begin(), ancestors.end());
        print_vector(ancestors);
        
        std::cout << "Descendants: ";
        std::vector<integer>& desc = descendants[i];
        if (!desc.empty())
        {
            std::sort(desc.begin(), desc.end());
            if (desc[0] == i)
                desc.erase(desc.begin());
        }
        std::cout << desc.size() << '\n';
        total_descendants += desc.size();
        if (!desc.empty())
            print_vector(desc);
        std::cout << '\n';
    }
    std::cout << "Total descendants: " << total_descendants << '\n';
    return 0;
}
