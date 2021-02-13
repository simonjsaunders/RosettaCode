#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <boost/circular_buffer.hpp>

// The input file must consist of one word per line in alphabetical order.
int main(int argc, char** argv) {
    const int min_length = 9;
    const char* filename(argc < 2 ? "unixdict.txt" : argv[1]);
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open file '" << filename << "'.\n";
        return EXIT_FAILURE;
    }
    std::string line;
    boost::circular_buffer<std::string> words(min_length);
    std::string previous_word;
    int count = 0;
    while (getline(in, line)) {
        if (line.size() < min_length)
            continue;
        words.push_back(line);
        if (words.size() < min_length)
            continue;
        std::string word;
        word.reserve(min_length);
        for (size_t i = 0; i < min_length; ++i)
            word += words[i][i];
        if (previous_word == word)
            continue;
        auto it = std::find(words.begin(), words.end(), word);
        if (it != words.end())
            std::cout << std::setw(2) << ++count << ". " << word << '\n';
        previous_word = word;
    }
    return EXIT_SUCCESS;
}
