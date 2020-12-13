#include <cstdlib>
#include <fstream>
#include <iostream>

bool is_abc_word(const std::string& word) {
    size_t a = std::string::npos;
    size_t b = std::string::npos;
    for (size_t pos = 0, n = word.size(); pos < n; ++pos) {
        switch (word[pos]) {
        case 'a':
            // record position of first 'a'
            if (a == std::string::npos)
                a = pos;
            break;
        case 'b':
            if (b == std::string::npos) {
                // fail if we haven't seen 'a' yet
                if (a == std::string::npos)
                    return false;
                // record position of first 'b'
                b = pos;
            }
            break;
        case 'c':
            // succeed iff we've seen 'b' already
            return b != std::string::npos;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    const char* filename(argc < 2 ? "unixdict.txt" : argv[1]);
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file '" << filename << "'.\n";
        return EXIT_FAILURE;
    }
    std::string word;
    int n = 1;
    while (getline(in, word)) {
        if (is_abc_word(word))
            std::cout << n++ << ": " << word << '\n';
    }
    return EXIT_SUCCESS;
}
