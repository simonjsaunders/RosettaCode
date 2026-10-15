#include <algorithm>
#include <cctype>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

class random_number_generator {
public:
    explicit random_number_generator(int low, int high);

    int number() { return distribution(engine); }

private:
    std::default_random_engine engine;
    std::uniform_int_distribution<int> distribution;
};

random_number_generator::random_number_generator(int low, int high)
    : engine(std::random_device{}()), distribution(low, high) {}

class word_generator {
public:
    explicit word_generator(const char* file_name);

    std::string next_word();

private:
    static std::vector<std::string> get_words(const char* file_name);
    static bool valid_word(const std::string& word);
    static std::string capitalize(const std::string& word);

    std::vector<std::string> words;
    random_number_generator rng;
};

class passphrase_generator {
public:
    passphrase_generator();
    std::string passphrase(int words);

private:
    word_generator word_gen;
    random_number_generator rng;
};

word_generator::word_generator(const char* file_name)
    : words(get_words(file_name)), rng(0, words.size() - 1) {}

std::string word_generator::next_word() {
    for (;;) {
        std::string word = words[rng.number()];
        if (valid_word(word))
            return capitalize(word);
    }
}

std::vector<std::string> word_generator::get_words(const char* file_name) {
    std::vector<std::string> words;
    std::ifstream in(file_name);
    if (!in)
        throw std::runtime_error(
            std::format("Cannot open file '{}'.", file_name));
    std::string word;
    while (getline(in, word))
        words.push_back(word);
    if (words.empty())
        throw std::runtime_error(std::format("File '{}' is empty.", file_name));
    return words;
}

bool word_generator::valid_word(const std::string& word) {
    return word.size() >= 4 &&
           std::all_of(word.begin(), word.end(),
                       [](unsigned char c) { return std::isalpha(c); });
}

std::string word_generator::capitalize(const std::string& word) {
    std::string result(word);
    if (!result.empty())
        result[0] = std::toupper(static_cast<unsigned char>(result[0]));
    return result;
}

passphrase_generator::passphrase_generator()
    : word_gen("unixdict.txt"), rng(10, 99) {}

std::string passphrase_generator::passphrase(int words) {
    std::string phrase;
    for (int i = 0; i < words; ++i) {
        phrase += word_gen.next_word() + std::to_string(rng.number());
        if (i < words - 1)
            phrase += "-";
    }
    return phrase;
}

int main() {
    try {
        passphrase_generator pg;
        for (int i = 0; i < 5; ++i)
            std::cout << pg.passphrase(5) << '\n';
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}
