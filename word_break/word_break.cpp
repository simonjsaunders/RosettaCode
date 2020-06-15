#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <string>

auto load_dictionary(const std::string& filename) {
    std::ifstream in(filename);
    if (!in)
        throw std::runtime_error("Cannot open file " + filename);
    std::set<std::string> words;
    std::string word;
    while (getline(in, word))
        words.insert(word);
    return words;
}

bool split_into_words(const std::set<std::string>& dict,
                      const std::string& str,
                      std::list<std::string>& words) {
    for (size_t len = str.size(); len > 0; --len) {
        auto word = str.substr(0, len);
        if (dict.find(word) == dict.end())
            continue;
        if (len == str.size()) {
            words.push_back(word);
            return true;
        } else if (split_into_words(dict, str.substr(len), words)) {
            words.push_front(word);
            return true;
        }
    }
    return false;
}

void print_words(const std::list<std::string>& words) {
    if (words.empty())
        return;
    auto i = words.begin();
    std::cout << *i++;
    for (; i != words.end(); ++i)
        std::cout << ' ' << *i;
    std::cout << '\n';
}

void test(const std::set<std::string>& dict, const std::string& input) {
    std::list<std::string> words;
    std::cout << "input: " << input << "\nresult: ";
    if (split_into_words(dict, input, words))
        print_words(words);
    else
        std::cout << "impossible!\n";
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " dictionary\n";
        return EXIT_FAILURE;
    }
    try {
        auto dict = load_dictionary(argv[1]);
        test(dict, "wordbreakproblem");
        test(dict, "segmenttheinputstring");
        test(dict, "1234");
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
