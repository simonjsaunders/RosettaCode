#include <array>
#include <iostream>
#include <map>
#include <vector>

enum class op { none, plus, minus };

class expression {
public:
    explicit expression() { ops.fill(op::none); }
    int sum() const;
    bool next();
    std::ostream& print(std::ostream& os) const;

private:
    std::array<op, 9> ops;
};

inline std::ostream& operator<<(std::ostream& os, const expression& e) {
    return e.print(os);
}

int expression::sum() const {
    int result = 0, n = 0, p = 1;
    for (int i = 9; i >= 1; --i) {
        n += i * p;
        switch (ops[i - 1]) {
        case op::none:
            p *= 10;
            break;
        case op::plus:
            p = 1;
            result += n;
            n = 0;
            break;
        case op::minus:
            p = 1;
            result -= n;
            n = 0;
            break;
        }
    }
    result += n;
    return result;
}

std::ostream& expression::print(std::ostream& os) const {
    for (int i = 0; i < 9; ++i) {
        switch (ops[i]) {
        case op::none:
            break;
        case op::plus:
            os << "+";
            break;
        case op::minus:
            os << "-";
            break;
        }
        os << i + 1;
    }
    return os;
}

bool expression::next() {
    for (int i = 8; i >= 0; --i) {
        switch (ops[i]) {
        case op::none:
            ops[i] = i == 0 ? op::minus : op::plus;
            return true;
        case op::plus:
            ops[i] = op::minus;
            return true;
        case op::minus:
            ops[i] = op::none;
            break;
        }
    }
    return false;
}

int main() {
    expression exp;
    std::map<int, std::vector<expression>> sums;

    do {
        sums[exp.sum()].push_back(exp);
    } while (exp.next());

    std::cout << "Solutions that sum to 100:\n";
    if (auto i = sums.find(100); i != sums.end()) {
        for (const auto& exp : i->second)
            std::cout << "100 = " << exp << '\n';
    }

    int max_count = 0;
    int max_sum;
    for (auto i = sums.begin(); i != sums.end(); ++i) {
        size_t count = i->second.size();
        if (count > max_count) {
            max_count = count;
            max_sum = i->first;
        }
    }
    std::cout << "\nThe sum with the greatest number of solutions is "
              << max_sum << " (" << max_count << ").\n";

    int n = 1;
    for (auto i = sums.lower_bound(1); i != sums.end(); ++i, ++n) {
        if (n != i->first)
            break;
    }
    std::cout << "\nThe smallest positive number that cannot be expressed is "
              << n << ".\n";

    std::cout << "\nThe ten highest numbers that can be expressed are:\n";
    int count = 0;
    for (auto i = sums.rbegin(); i != sums.rend() && count++ < 10; ++i)
        std::cout << i->first << " = " << i->second[0] << '\n';
}
