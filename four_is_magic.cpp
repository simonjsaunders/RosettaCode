#include <iostream>
#include <string>
#include <cctype>
#include <cstdint>

typedef std::uint64_t integer;

const char* small[] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight",
    "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

const char* tens[] = {
    "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};

struct named_number
{
    const char* name_;
    integer number_;
};

named_number named_numbers[] = {
    { "hundred", 100 },
    { "thousand", 1000 },
    { "million", 1000000 },
    { "billion", 1000000000 },
    { "trillion", 1000000000000 },
    { "quadrillion", 1000000000000000ULL },
    { "quintillion", 1000000000000000000ULL }
};
const size_t names_len = sizeof(named_numbers)/sizeof(named_numbers[0]);

std::string cardinal(integer n)
{
    std::string result;
    if (n < 20)
        result = small[n];
    else if (n < 100)
    {
        result = tens[n/10 - 2];
        if (n % 10 != 0)
        {
            result += " ";
            result += small[n % 10];
        }
    }
    else
    {
        for (size_t i = 1; i <= names_len; ++i)
        {
            if (i == names_len || n < named_numbers[i].number_)
            {
                integer p = named_numbers[i-1].number_;
                result = cardinal(n/p);
                result += " ";
                result += named_numbers[i-1].name_;
                if (n % p != 0)
                {
                    result += " ";
                    result += cardinal(n % p);
                }
                break;
            }
        }
    }
    return result;
}

inline char uppercase(char ch)
{
    return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

void magic(integer n)
{
    for (unsigned int i = 0; ; ++i)
    {
        std::string text(cardinal(n));
        if (i == 0)
            text[0] = uppercase(text[0]);
        if (n == 4)
        {
            std::cout << text << " is magic.\n";
            break;
        }
        integer len = text.length();
        std::cout << text << " is " << cardinal(len) << ", ";
        n = len;
    }
}

int main()
{
    magic(5);
    magic(13);
    magic(78);
    magic(797);
    magic(2739);
    magic(4000);
    magic(7893);
    magic(93497412);
    magic(2673497412U);
    magic(10344658531277200972ULL);
    return 0;
}
