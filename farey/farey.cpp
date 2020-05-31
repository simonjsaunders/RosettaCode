#include <iostream>

void print_fraction(int a, int b) {
    std::cout << a << '/' << b;
}

//
// See https://en.wikipedia.org/wiki/Farey_sequence#Next_term
//
void print_farey_sequence(int n, bool length_only) {
    std::cout << n << ": ";
    int a = 0, b = 1, c = 1, d = n;
    if (!length_only)
        print_fraction(a, b);
    int count = 1;
    for (; c <= n; ++count) {
        int k = (n + b)/d;
        int next_c = k * c - a;
        int next_d = k * d - b;
        a = c;
        b = d;
        c = next_c;
        d = next_d;
        if (!length_only) {
            std::cout << ' ';
            print_fraction(a, b);
        }
    }
    if (length_only)
        std::cout << count << " items";
    std::cout << '\n';
}

int main() {
    for (int i = 1; i <= 11; ++i)
        print_farey_sequence(i, false);
    for (int i = 100; i <= 1000; i += 100)
        print_farey_sequence(i, true);
    return 0;
}
