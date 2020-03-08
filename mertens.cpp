#include <iomanip>
#include <iostream>
#include <map>

class mertens_calculator
{
public:
    int mertens_number(int);
private:
    std::map<int, int> cache_;
};

int mertens_calculator::mertens_number(int n)
{
    auto i = cache_.find(n);
    if (i != cache_.end())
        return i->second;
    int m = 1;
    for (int k = 2; k <= n; ++k)
        m -= mertens_number(n/k);
    cache_.emplace(n, m);
    return m;
}

void print_mertens_numbers(mertens_calculator& mc, int count)
{
    int column = 0;
    for (int i = 0; i < count; ++i)
    {
        if (column > 0)
            std::cout << ' ';
        if (i == 0)
            std::cout << "  ";
        else
            std::cout << std::setw(2) << mc.mertens_number(i);
        ++column;
        if (column == 20)
        {
            std::cout << '\n';
            column = 0;
        }
    }
}

int main()
{
    mertens_calculator mc;
    std::cout << "First 199 Mertens numbers:\n";
    print_mertens_numbers(mc, 200);
    int zero = 0, cross = 0, previous = 0;
    for (int i = 1; i <= 1000; ++i)
    {
        int m = mc.mertens_number(i);
        if (m == 0)
        {
            ++zero;
            if (previous != 0)
                ++cross;
        }
        previous = m;
    }
    std::cout << "M(n) is zero " << zero << " times for 1 <= n <= 1000.\n";
    std::cout << "M(n) crosses zero " << cross << " times for 1 <= n <= 1000.\n";
    return 0;
}
