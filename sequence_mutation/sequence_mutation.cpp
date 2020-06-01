#include <iomanip>
#include <iostream>
#include <random>
#include <string>

class sequence_generator {
public:
    sequence_generator();
    std::string generate_sequence(size_t length);
    void mutate_sequence(std::string&);
    static void print_sequence(std::ostream&, const std::string&);
private:
    enum class operation { change, erase, insert };
    char get_random_base() {
        return bases_[base_dist_(engine_)];
    }
    operation get_random_operation() {
        return static_cast<operation>(operation_dist_(engine_));
    }
    static const char bases_[];
    std::mt19937 engine_;
    std::uniform_int_distribution<size_t> base_dist_;
    std::uniform_int_distribution<int> operation_dist_;
};

const char sequence_generator::bases_[] = { 'A', 'C', 'G', 'T' };

sequence_generator::sequence_generator() : engine_(std::random_device()()),
    base_dist_(0, sizeof(bases_)/sizeof(bases_[0]) - 1),
    operation_dist_(0, 2) {}

std::string sequence_generator::generate_sequence(size_t length) {
    std::string sequence;
    sequence.reserve(length);
    for (size_t i = 0; i < length; ++i)
        sequence += get_random_base();
    return sequence;
}

void sequence_generator::mutate_sequence(std::string& sequence) {
    std::uniform_int_distribution<size_t> dist(0, sequence.length() - 1);
    size_t pos = dist(engine_);
    char b;
    switch (get_random_operation()) {
    case operation::change:
        b = get_random_base();
        std::cout << "Change base at position " << pos << " from "
            << sequence[pos] << " to " << b << '\n';
        sequence[pos] = b;
        break;
    case operation::erase:
        std::cout << "Erase base " << sequence[pos] << " at position "
            << pos << '\n';
        sequence.erase(pos, 1);
        break;
    case operation::insert:
        b = get_random_base();
        std::cout << "Insert base " << b << " at position "
            << pos << '\n';
        sequence.insert(pos, 1, b);
        break;
    }
}

void sequence_generator::print_sequence(std::ostream& out, const std::string& sequence) {
    constexpr size_t base_count = sizeof(bases_)/sizeof(bases_[0]);
    size_t n = sequence.length();
    size_t count[base_count] = { 0 };
    for (size_t i = 0; i < n; ++i) {
        if (i % 50 == 0) {
            if (i != 0)
                out << '\n';
            out << std::setw(3) << i << ": ";
        }
        out << sequence[i];
        for (size_t j = 0; j < base_count; ++j) {
            if (bases_[j] == sequence[i]) {
                ++count[j];
                break;
            }
        }
    }
    out << '\n';
    out << "Base counts:\n";
    size_t total = 0;
    for (size_t j = 0; j < base_count; ++j) {
        total += count[j];
        out << bases_[j] << ": " << count[j] << ", ";
    }
    out << "Total: " << total << '\n';
}

int main() {
    sequence_generator gen;
    std::string sequence = gen.generate_sequence(250);
    std::cout << "Initial sequence:\n";
    sequence_generator::print_sequence(std::cout, sequence);
    constexpr int count = 10;
    for (int i = 0; i < count; ++i)
        gen.mutate_sequence(sequence);
    std::cout << "After " << count << " mutations:\n";
    sequence_generator::print_sequence(std::cout, sequence);
    return 0;
}
