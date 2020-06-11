#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "string_buffer.h"

typedef uint64_t integer;

typedef struct number_names_tag {
    const char* cardinal;
    const char* ordinal;
} number_names;

const number_names small[] = {
    { "zero", "zeroth" }, { "one", "first" }, { "two", "second" },
    { "three", "third" }, { "four", "fourth" }, { "five", "fifth" },
    { "six", "sixth" }, { "seven", "seventh" }, { "eight", "eighth" },
    { "nine", "ninth" }, { "ten", "tenth" }, { "eleven", "eleventh" },
    { "twelve", "twelfth" }, { "thirteen", "thirteenth" },
    { "fourteen", "fourteenth" }, { "fifteen", "fifteenth" },
    { "sixteen", "sixteenth" }, { "seventeen", "seventeenth" },
    { "eighteen", "eighteenth" }, { "nineteen", "nineteenth" }
};

const number_names tens[] = {
    { "twenty", "twentieth" }, { "thirty", "thirtieth" },
    { "forty", "fortieth" }, { "fifty", "fiftieth" },
    { "sixty", "sixtieth" }, { "seventy", "seventieth" },
    { "eighty", "eightieth" }, { "ninety", "ninetieth" }
};

typedef struct named_number_tag {
    const char* cardinal;
    const char* ordinal;
    integer number;
} named_number;

const named_number named_numbers[] = {
    { "hundred", "hundredth", 100 },
    { "thousand", "thousandth", 1000 },
    { "million", "millionth", 1000000 },
    { "billion", "billionth", 1000000000 },
    { "trillion", "trillionth", 1000000000000 },
    { "quadrillion", "quadrillionth", 1000000000000000ULL },
    { "quintillion", "quintillionth", 1000000000000000000ULL }
};

const char* get_small_name(const number_names* n, bool ordinal) {
    return ordinal ? n->ordinal : n->cardinal;
}

const char* get_big_name(const named_number* n, bool ordinal) {
    return ordinal ? n->ordinal : n->cardinal;
}

const named_number* get_named_number(integer n) {
    const size_t names_len = sizeof(named_numbers)/sizeof(named_numbers[0]);
    for (size_t i = 0; i + 1 < names_len; ++i) {
        if (n < named_numbers[i + 1].number)
            return &named_numbers[i];
    }
    return &named_numbers[names_len - 1];
}

void append_number_name(string_buffer* sb, integer n, bool ordinal) {
    if (n < 20)
        string_buffer_append_str(sb, get_small_name(&small[n], ordinal));
    else if (n < 100) {
        if (n % 10 == 0) {
            string_buffer_append_str(sb, get_small_name(&tens[n/10 - 2], ordinal));
        } else {
            string_buffer_append_str(sb, get_small_name(&tens[n/10 - 2], false));
            string_buffer_append(sb, '-');
            string_buffer_append_str(sb, get_small_name(&small[n % 10], ordinal));
        }
    } else {
        const named_number* num = get_named_number(n);
        integer p = num->number;
        append_number_name(sb, n/p, false);
        string_buffer_append(sb, ' ');
        if (n % p == 0) {
            string_buffer_append_str(sb, get_big_name(num, ordinal));
        } else {
            string_buffer_append_str(sb, get_big_name(num, false));
            string_buffer_append(sb, ' ');
            append_number_name(sb, n % p, ordinal);
        }
    }
}

char* number_name(integer n, bool ordinal) {
    string_buffer result = { 0 };
    string_buffer_create(&result, 8);
    append_number_name(&result, n, ordinal);
    return result.string;
}

void test_ordinal(integer n) {
    char* name = number_name(n, true);
    printf("%llu: %s\n", n, name);
    free(name);
}

int main() {
    test_ordinal(1);
    test_ordinal(2);
    test_ordinal(3);
    test_ordinal(4);
    test_ordinal(5);
    test_ordinal(11);
    test_ordinal(15);
    test_ordinal(21);
    test_ordinal(42);
    test_ordinal(65);
    test_ordinal(98);
    test_ordinal(100);
    test_ordinal(101);
    test_ordinal(272);
    test_ordinal(300);
    test_ordinal(750);
    test_ordinal(23456);
    test_ordinal(7891233);
    test_ordinal(8007006005004003LL);
    return 0;
}
