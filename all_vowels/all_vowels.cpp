#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

bool contains_all_vowels_once(const std::string& word) {
    unsigned int vowels = 0;
    for (char ch : word) {
        unsigned int bit = 0;
        switch (ch) {
        case 'a':
            bit = 1;
            break;
        case 'e':
            bit = 2;
            break;
        case 'i':
            bit = 4;
            break;
        case 'o':
            bit = 8;
            break;
        case 'u':
            bit = 16;
            break;
        }
        if (bit == 0)
            continue;
        if (vowels & bit)
            return false;
        vowels |= bit;
    }
    return vowels == 31;
}

int main(int argc, char** argv) {
    const char* filename(argc < 2 ? "unixdict.txt" : argv[1]);
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file '" << filename << "'.\n";
        return EXIT_FAILURE;
    }
    std::string word;
    int n = 0;
    while (getline(in, word)) {
        if (word.size() > 10 && contains_all_vowels_once(word))
            std::cout << std::setw(2) << ++n << ": " << word << '\n';
    }
    return EXIT_SUCCESS;
}
