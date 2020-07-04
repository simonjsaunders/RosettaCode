#include <array>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

// A multiset specialized for strings consisting of lowercase
// letters ('a' to 'z').
class letterset {
public:
    explicit letterset(const std::string& str) {
        count_.fill(0);
        for (char c : str) {
            if (c >= 'a' && c <= 'z')
                ++count_[c - 'a' + 1];
            else
                ++count_[0];
        }
    }
    bool contains(const letterset& set) const {
        for (size_t i = 0; i < count_.size(); ++i) {
            if (set.count_[i] > count_[i])
                return false;
        }
        return true;
    }
    unsigned int count(char c) const {
        if (c >= 'a' && c <= 'z')
            return count_[c - 'a' + 1];
        return 0;
    }
    bool is_valid() const {
        return count_[0] == 0;
    }
private:
    // elements 1..26 contain the number of times each lowercase
    // letter occurs in the word
    // element 0 is the number of other characters in the word
    std::array<unsigned int, 27> count_;
};

template <typename iterator, typename separator>
std::string join(iterator begin, iterator end, separator sep) {
    std::string result;
    if (begin != end) {
        result += *begin++;
        for (; begin != end; ++begin) {
            result += sep;
            result += *begin;
        }
    }
    return result;
}

int main(int argc, char** argv) {
    const int min_length = 3;
    const char* letters = "ndeokgelw";
    const char central_letter = 'k';
    const char* dict(argc == 2 ? argv[1] : "unixdict.txt");
    std::ifstream in(dict);
    if (!in) {
        std::cerr << "Cannot open file " << dict << '\n';
        return 1;
    }
    letterset set(letters);
    std::string word;
    std::map<size_t, std::vector<std::string>> words;
    while (getline(in, word)) {
        if (word.size() < min_length)
            continue;
        letterset subset(word);
        if (subset.count(central_letter) > 0 && set.contains(subset)) {
            words[word.size()].push_back(word);
        }
    }
    for (const auto& p : words) {
        const auto& v = p.second;
        auto n = v.size();
        std::cout << "Found " << n << " " << (n == 1 ? "word" : "words")
            << " of length " << p.first << ": "
            << join(v.begin(), v.end(), ", ") << '\n';
    }
    return 0;
}
