#include <iostream>
#include <cstdint>
#include <queue>
#include <utility>
#include <vector>
#include <limits>

template<typename integer>
class prime_generator
{
public:
    prime_generator() : next_(2), count_(0)
    {
    }
    integer next_prime();
    integer count() const
    {
        return count_;
    }
private:
    typedef std::pair<integer, integer> pair;
    struct cmp
    {
        bool operator()(const pair& p1, const pair& p2) const
        {
            return p1.first > p2.first;
        }
    };
    typedef std::priority_queue<pair, std::vector<pair>, cmp> queue;
    integer next_;
    integer count_;
    queue queue_;
};

template<typename integer>
integer prime_generator<integer>::next_prime()
{
    integer n = next_;
    integer prev = 0;
    while (!queue_.empty())
    {
        pair p = queue_.top();
        if (prev != 0 && prev != p.first)
            ++n;
        if (p.first > n)
            break;
        else if (p.first == n)
        {
            queue_.pop();
            queue_.emplace(p.second + p.first, p.second);
        }
        else
            throw std::overflow_error("prime_generator: overflow!");
        prev = p.first;
    }
    if (std::numeric_limits<integer>::max()/n > n)
        queue_.emplace(n * n, n);
    next_ = n + 1;
    ++count_;
    return n;
}

int main()
{
    typedef uint32_t integer;
    prime_generator<integer> pgen;
    std::cout << "First 20 primes:\n";
    for (int i = 0; i < 20; ++i)
    {
        integer p = pgen.next_prime();
        if (i != 0)
            std::cout << ", ";
        std::cout << p;
    }
    std::cout << "\nPrimes between 100 and 150:\n";
    for (int n = 0; ; )
    {
        integer p = pgen.next_prime();
        if (p > 150)
            break;
        if (p >= 100)
        {
            if (n != 0)
                std::cout << ", ";
            std::cout << p;
            ++n;
        }
    }
    int count = 0;
    for (;;)
    {
        integer p = pgen.next_prime();
        if (p > 8000)
            break;
        if (p >= 7700)
            ++count;
    }
    std::cout << "\nNumber of primes between 7700 and 8000: " << count << '\n';

    integer prime;
    while (pgen.count() != 10000)
        prime = pgen.next_prime();
    std::cout << "10000th prime: " << prime << '\n';
    return 0;
}
