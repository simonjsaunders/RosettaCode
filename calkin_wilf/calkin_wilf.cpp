#include <iostream>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <boost/rational.hpp>

using rational = boost::rational<unsigned long>;
using bitset = boost::dynamic_bitset<unsigned long>;

unsigned long floor(const rational& r) {
    return r.numerator()/r.denominator();
}

class calkin_wilf {
public:
    rational next() {
        rational term = term_;
        term_ = 1UL/(2UL * floor(term) + 1UL - term);
        return term;
    }
private:
    rational term_ = 0;
};

std::vector<unsigned long> continued_fraction(const rational& r) {
    unsigned long a = r.numerator();
    unsigned long b = r.denominator();
    std::vector<unsigned long> result;
    for (;;) {
        result.push_back(a/b);
        unsigned long c = a;
        a = b;
        b = c % b;
        if (a == 1)
            break;
    }
    if (result.size() > 0 && result.size() % 2 == 0) {
        --result.back();
        result.push_back(1);
    }
    return result;
}

unsigned long term_number(const rational& r) {
    bitset b;
    bool d = true;
    for (unsigned long n : continued_fraction(r)) {
        for (unsigned long i = 0; i < n; ++i)
            b.push_back(d);
        d = !d;
    }
    return b.to_ulong();
}

int main() {
    calkin_wilf cf;
    std::cout << "First 21 terms of the Calkin-Wilf sequence are:\n";
    for (int i = 0; i <= 20; ++i) {
        rational term = cf.next();
        std::cout << std::setw(2) << i << ": " << term << '\n';
    }
    rational r(83116, 51639);
    std::cout << r << " is the " << term_number(r) << "th term of the sequence.\n";
}
