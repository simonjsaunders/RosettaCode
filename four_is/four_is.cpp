#include <cctype>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

typedef std::uint64_t integer;

struct number_names {
    const char* cardinal;
    const char* ordinal;
};

number_names small[] = {
    { "zero", "zeroth" }, { "one", "first" }, { "two", "second" },
    { "three", "third" }, { "four", "fourth" }, { "five", "fifth" },
    { "six", "sixth" }, { "seven", "seventh" }, { "eight", "eighth" },
    { "nine", "ninth" }, { "ten", "tenth" }, { "eleven", "eleventh" },
    { "twelve", "twelfth" }, { "thirteen", "thirteenth" },
    { "fourteen", "fourteenth" }, { "fifteen", "fifteenth" },
    { "sixteen", "sixteenth" }, { "seventeen", "seventeenth" },
    { "eighteen", "eighteenth" }, { "nineteen", "nineteenth" }
};

number_names tens[] = {
    { "twenty", "twentieth" }, { "thirty", "thirtieth" },
    { "forty", "fortieth" }, { "fifty", "fiftieth" },
    { "sixty", "sixtieth" }, { "seventy", "seventieth" },
    { "eighty", "eightieth" }, { "ninety", "ninetieth" }
};

struct named_number {
    const char* cardinal;
    const char* ordinal;
    integer number;
};

named_number named_numbers[] = {
    { "hundred", "hundredth", 100 },
    { "thousand", "thousandth", 1000 },
    { "million", "millionth", 1000000 },
    { "billion", "biliionth", 1000000000 },
    { "trillion", "trillionth", 1000000000000 },
    { "quadrillion", "quadrillionth", 1000000000000000ULL },
    { "quintillion", "quintillionth", 1000000000000000000ULL }
};

const char* get_name(const number_names& n, bool ordinal) {
    return ordinal ? n.ordinal : n.cardinal;
}

const char* get_name(const named_number& n, bool ordinal) {
    return ordinal ? n.ordinal : n.cardinal;
}

void append(std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    v1.insert(v1.end(), v2.begin(), v2.end());
}

std::vector<std::string> number_name(integer n, bool ordinal) {
    std::vector<std::string> result;
    if (n < 20)
        result.push_back(get_name(small[n], ordinal));
    else if (n < 100) {
        if (n % 10 == 0) {
            result.push_back(get_name(tens[n/10 - 2], ordinal));
        } else {
            std::string name(get_name(tens[n/10 - 2], false));
            name += "-";
            name += get_name(small[n % 10], ordinal);
            result.push_back(name);
        }
    } else {
        constexpr size_t names_len = std::size(named_numbers);
        for (size_t i = 1; i <= names_len; ++i) {
            if (i == names_len || n < named_numbers[i].number) {
                integer p = named_numbers[i-1].number;
                append(result, number_name(n/p, false));
                if (n % p == 0) {
                    result.push_back(get_name(named_numbers[i-1], ordinal));
                } else {
                    result.push_back(get_name(named_numbers[i-1], false));
                    append(result, number_name(n % p, ordinal));
                }
                break;
            }
        }
    }
    return result;
}

const char* words[] = {
    "Four", "is", "the", "number", "of", "letters", "in", "the",
    "first", "word", "of", "this", "sentence,"
};

size_t count_letters(const std::string& str) {
    size_t letters = 0;
    for (size_t i = 0, n = str.size(); i < n; ++i) {
        if (isalpha(static_cast<unsigned char>(str[i])))
            ++letters;
    }
    return letters;
}

std::vector<std::string> sentence(size_t count) {
    std::vector<std::string> result;
    size_t n = std::size(words);
    for (size_t i = 0; i < n && i < count; ++i) {
        result.push_back(words[i]);
    }
    for (size_t i = 1; count > n; ++i) {
        std::vector<std::string> v;
        append(v, number_name(count_letters(result[i]), false));
        v.push_back("in");
        v.push_back("the");
        append(v, number_name(i + 1, true));
        v.back() += ',';
        n += v.size();
        append(result, v);
    }
    return result;
}

int main() {
    std::cout.imbue(std::locale(""));
    auto result = sentence(10000000);
    size_t characters = 0;
    size_t i = 0;
    std::cout << "Number of letters in first 201 words in the sequence:\n";
    for (; i < 201; ++i) {
        if (i != 0) {
            std::cout << ((i % 25 == 0) ? '\n' : ' ');
        }
        characters += result[i].size();
        if (i != 0)
            ++characters;
        std::cout << std::setw(2) << count_letters(result[i]);
    }
    std::cout << '\n';
    std::cout << "Number of characters: " << characters << '\n';
    for (size_t n = 1000; i < 10000000; ++i) {
        characters += result[i].size() + 1;
        if (i + 1 == n) {
            const std::string& word = result[i];
            std::cout << "The " << n << "th word is '" << word << "' and has "
                << count_letters(word) << " letters. ";
            std::cout << "Number of characters: " << characters << '\n';
            n *= 10;
        }
    }
    return 0;
}
