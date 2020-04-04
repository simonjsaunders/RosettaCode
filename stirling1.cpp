#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>

class unsigned_stirling1
{
public:
    int get(int n, int k);
private:
    std::map<std::pair<int, int>, int> cache_;
};

int unsigned_stirling1::get(int n, int k)
{
    if (k == 0)
        return n == 0 ? 1 : 0;
    if (k > n)
        return 0;
    auto p = std::make_pair(n, k);
    auto i = cache_.find(p);
    if (i != cache_.end())
        return i->second;
    int s = get(n - 1, k - 1) + (n - 1) * get(n - 1, k);
    cache_.emplace(p, s);
    return s;
}

void print_stirling_numbers(unsigned_stirling1& s1, int n)
{
    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= i; ++j)
            std::cout << std::setw(10) << s1.get(i, j);
        std::cout << '\n';
    }
}

int main()
{
    unsigned_stirling1 s1;
    std::cout << "Unsigned Stirling numbers of the first kind:\n";
    print_stirling_numbers(s1, 12);
    return 0;
}
