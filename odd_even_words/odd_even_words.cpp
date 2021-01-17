#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

bool is_vowel(char ch) {
    switch (ch) {
    case 'a': case 'A':
    case 'e': case 'E':
    case 'i': case 'I':
    case 'o': case 'O':
    case 'u': case 'U':
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    const char* filename = argc < 2 ? "unixdict.txt" : argv[1];
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file '" << filename << "'.\n";
        return EXIT_FAILURE;
    }
    std::string line;
    for (int n = 1; getline(in, line); ) {
        size_t len = line.size();
        if (len <= 9)
            continue;
        bool is_odd_even_word = true;
        for (size_t i = 1; i < len; ++i) {
            if (is_vowel(line[i]) == is_vowel(line[i - 1])) {
                is_odd_even_word = false;
                break;
            }
        }
        if (is_odd_even_word)
            std::cout << std::setw(2) << n++ << ": " << line << '\n';
    }
    return EXIT_SUCCESS;
}
