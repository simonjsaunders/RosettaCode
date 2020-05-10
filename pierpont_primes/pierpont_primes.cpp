#include <cassert>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>
#include <gmpxx.h>

using big_int = mpz_class;

bool is_prime(const big_int& n)
{
    return mpz_probab_prime_p(n.get_mpz_t(), 25);
}

class n_smooth_generator
{
public:
    explicit n_smooth_generator(int n);
    big_int next();
private:
    std::vector<int> primes_;
    std::set<big_int> numbers_;
};

n_smooth_generator::n_smooth_generator(int n)
{
    for (int p = 2; p <= n; ++p)
    {
        if (is_prime(p))
            primes_.push_back(p);
    }
    numbers_.insert(1);
}

big_int n_smooth_generator::next()
{
    assert(!numbers_.empty());
    big_int result = *numbers_.begin();
    numbers_.erase(numbers_.begin());
    for (auto prime : primes_)
        numbers_.insert(result * prime);
    return result;
}

void print_vector(const std::vector<big_int>& numbers)
{
    for (size_t i = 0, n = numbers.size(); i < n; ++i)
    {
        std::cout << std::setw(9) << numbers[i];
        if ((i + 1) % 10 == 0)
            std::cout << '\n';
    }
    std::cout << '\n';
}

int main()
{
    const int max = 250;
    std::vector<big_int> first, second;
    int count1 = 0;
    int count2 = 0;
    n_smooth_generator smooth_gen(3);
    big_int p1, p2;

    while (count1 < max || count2 < max)
    {
        big_int n = smooth_gen.next();
        if (count1 < max && is_prime(n + 1))
        {
            p1 = n + 1;
            if (first.size() < 50)
                first.push_back(p1);
            ++count1;
        }
        if (count2 < max && is_prime(n - 1))
        {
            p2 = n - 1;
            if (second.size() < 50)
                second.push_back(p2);
            ++count2;
        }
    }
    
    std::cout << "First 50 Pierpont primes of the first kind:\n";
    print_vector(first);
    std::cout << "First 50 Pierpont primes of the second kind:\n";
    print_vector(second);
    
    std::cout << "250th Pierpont prime of the first kind: " << p1 << '\n';
    std::cout << "250th Pierpont prime of the second kind: " << p2 << '\n';

    return 0;
}
