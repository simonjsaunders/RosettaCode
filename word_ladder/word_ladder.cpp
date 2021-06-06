#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using word_map = std::map<size_t, std::vector<std::string>>;

// Returns true if strings s1 and s2 differ by one character.
bool one_away(const std::string& s1, const std::string& s2) {
    if (s1.size() != s2.size())
        return false;
    int sum = 0;
    for (size_t i = 0, n = s1.size(); i != n; ++i) {
        if (s1[i] != s2[i]) {
            if (++sum > 1)
                return false;
        }
    }
    return sum == 1;
}

// Join a sequence of strings into a single string using the given separator.
template <typename iterator_type, typename separator_type>
std::string join(iterator_type begin, iterator_type end,
                 separator_type separator) {
    std::string result;
    if (begin != end) {
        result += *begin++;
        for (; begin != end; ++begin) {
            result += separator;
            result += *begin;
        }
    }
    return result;
}

// Return true if v contains e.
template <typename vector_type, typename element_type>
bool contains(const vector_type& v, const element_type& e) {
    return std::find(v.begin(), v.end(), e) != v.end();
}

// If possible, print the shortest chain of single-character modifications that
// leads from "from" to "to", with each intermediate step being a valid word.
// This is an application of breadth-first search.
bool word_ladder(const word_map& words, const std::string& from,
                 const std::string& to) {
    auto w = words.find(from.size());
    if (w != words.end()) {
        auto poss = w->second;
        std::vector<std::vector<std::string>> queue{{from}};
        while (!queue.empty()) {
            auto curr = queue.front();
            queue.erase(queue.begin());
            std::vector<std::string> next;
            for (const std::string& str : poss) {
                if (one_away(str, curr.back()))
                    next.push_back(str);
            }
            if (contains(next, to)) {
                curr.push_back(to);
                std::cout << join(curr.begin(), curr.end(), " -> ") << '\n';
                return true;
            }
            poss.erase(std::remove_if(poss.begin(), poss.end(),
                                      [&next](const std::string& str) {
                                          return contains(next, str);
                                      }),
                       poss.end());
            for (const auto& str : next) {
                std::vector<std::string> temp(curr);
                temp.push_back(str);
                queue.push_back(std::move(temp));
            }
        }
    }
    std::cout << from << " into " << to << " cannot be done.\n";
    return false;
}

int main() {
    word_map words;
    std::ifstream in("unixdict.txt");
    if (!in) {
        std::cerr << "Cannot open file unixdict.txt.\n";
        return EXIT_FAILURE;
    }
    std::string word;
    while (getline(in, word))
        words[word.size()].push_back(word);
    word_ladder(words, "boy", "man");
    word_ladder(words, "girl", "lady");
    word_ladder(words, "john", "jane");
    word_ladder(words, "child", "adult");
    word_ladder(words, "cat", "dog");
    word_ladder(words, "lead", "gold");
    word_ladder(words, "white", "black");
    word_ladder(words, "bubble", "tickle");
    return EXIT_SUCCESS;
}