#include <iostream>
#include <string>

std::string base2(unsigned int n) {
    std::string s;
    do {
        s.insert(s.begin(), '0' + (n & 1));
        n >>= 1;
    } while (n != 0);
    return s;
}

int main() {
    std::cout << "Decimal\tBinary\n";
    for (unsigned int n = 1; ; ++n) {
        std::string s(base2(n));
        unsigned int i = n + (n << s.size());
        if (i >= 1000)
            break;
        std::cout << i << '\t' << s << s << '\n';
    }
}
