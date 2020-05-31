#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::cpp_int integer;

integer mod(const integer& x, const integer& y) {
    return ((x % y) + y) % y;
}

size_t count_digits(const integer& i) {
    std::ostringstream os;
    os << i;
    return os.str().length();
}

std::string integer_to_string(const integer& i) {
    const int max_digits = 20;
    std::ostringstream os;
    os << i;
    std::string s = os.str();
    if (s.length() > max_digits) {
        s = s.substr(0, max_digits/2) + "..." + s.substr(s.length()-max_digits/2);
    }
    return s;
}

void egyptian(integer x, integer y, std::vector<integer>& result) {
    result.clear();
    while (x > 0) {
        integer z = (y + x - 1)/x;
        result.push_back(z);
        x = mod(-y, x);
        y = y * z;
    }
}

void print_egyptian(const std::vector<integer>& result) {
    if (result.empty())
        return;
    auto i = result.begin();
    std::cout << "1/" << integer_to_string(*i++);
    for (; i != result.end(); ++i)
        std::cout << " + 1/" << integer_to_string(*i);
    std::cout << "\n\n";
}

void print_egyptian(integer x, integer y) {
    std::cout << "Egyptian fraction for " << x << "/" << y << ": ";
    if (x > y) {
        std::cout << "[" << x/y << "] ";
        x = x % y;
    }
    std::vector<integer> result;
    egyptian(x, y, result);
    print_egyptian(result);
}

void show_max_terms_and_max_denominator(const integer& limit) {
    size_t max_terms = 0;
    integer max_terms_numerator, max_terms_denominator;
    integer max_denominator_numerator, max_denominator_denominator;
    std::vector<integer> max_terms_result;
    integer max_denominator = 0;
    std::vector<integer> max_denominator_result;
    std::vector<integer> result;
    for (integer b = 2; b < limit; ++b) {
        for (integer a = 1; a < b; ++a) {
            egyptian(a, b, result);
            if (result.size() > max_terms) {
                max_terms = result.size();
                max_terms_result = result;
                max_terms_numerator = a;
                max_terms_denominator = b;
            }
            integer largest_denominator = result.back();
            if (largest_denominator > max_denominator) {
                max_denominator = largest_denominator;
                max_denominator_result = result;
                max_denominator_numerator = a;
                max_denominator_denominator = b;
            }
        }
    }
    std::cout << "Proper fractions with most terms and largest denominator, limit = " << limit << ":\n\n";
    std::cout << "Most terms (" << max_terms << "): " << max_terms_numerator
        << '/' << max_terms_denominator << " = ";
    print_egyptian(max_terms_result);
    std::cout << "Largest denominator (" << count_digits(max_denominator_result.back()) << " digits): "
        << max_denominator_numerator << '/' << max_denominator_denominator << " = ";
    print_egyptian(max_denominator_result);
}

int main() {
    print_egyptian(43, 48);
    print_egyptian(5, 121);
    print_egyptian(2014, 59);
    show_max_terms_and_max_denominator(100);
    show_max_terms_and_max_denominator(1000);
    return 0;
}

