#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Returns number of consonants in the word if they are all unique,
// otherwise zero.
int consonants(const std::string& word) {
    int total = 0;
    int count[26] = {};
    for (char ch : word) {
        ch = std::tolower(static_cast<unsigned char>(ch));
        if (ch < 'a' || ch > 'z')
            continue;
        switch (ch) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            break;
        default:
            if (count[ch - 'a']++ != 0)
                return 0;
            ++total;
            break;
        }
    }
    return total;
}

int main(int argc, char** argv) {
    const char* filename(argc < 2 ? "unixdict.txt" : argv[1]);
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file '" << filename << "'.\n";
        return EXIT_FAILURE;
    }
    std::string word;
    std::map<int, std::vector<std::string>, std::greater<int>> map;
    while (getline(in, word)) {
        if (word.size() <= 10)
            continue;
        int count = consonants(word);
        if (count > 0)
            map[count].push_back(word);
    }
    const int columns = 4;
    for (const auto& p : map) {
        std::cout << p.first << " consonants (" << p.second.size() << "):\n";
        int n = 0;
        for (const auto& word : p.second) {
            std::cout << std::left << std::setw(18) << word;
            ++n;
            if (n % columns == 0)
                std::cout << '\n';
        }
        if (n % columns != 0)
            std::cout << '\n';
        std::cout << '\n';
    }
    return EXIT_SUCCESS;
}
