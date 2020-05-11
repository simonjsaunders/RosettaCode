#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t integer;

typedef struct number_names_tag {
    const char* cardinal;
    const char* ordinal;
} number_names;

number_names small[] = {
    { "zero", "zeroth" }, { "one", "first" }, { "two", "second" },
    { "three", "third" }, { "four", "fourth" }, { "five", "fifth" },
    { "six", "sixth" }, { "seven", "seventh" }, { "eight", "eighth" },
    { "nine", "ninth" }, { "ten", "tenth" }, { "eleven", "tenth" },
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

typedef struct named_number_tag {
    const char* cardinal;
    const char* ordinal;
    integer number;
} named_number;

named_number named_numbers[] = {
    { "hundred", "hundredth", 100 },
    { "thousand", "thousandth", 1000 },
    { "million", "millionth", 1000000 },
    { "billion", "biliionth", 1000000000 },
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

typedef struct string_builder_tag {
    size_t size;
    size_t capacity;
    char* string;
} string_builder;

void string_builder_append(string_builder* sb, const char* str) {
    size_t n = strlen(str);
    size_t min_capacity = sb->size + n + 1;
    if (sb->capacity < min_capacity) {
        size_t new_capacity = sb->capacity * 2;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        char* new_string = realloc(sb->string, new_capacity);
        if (new_string == NULL) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
        sb->string = new_string;
        sb->capacity = new_capacity;
    }
    memcpy(sb->string + sb->size, str, n + 1);
    sb->size += n;
}

void append_number_name(string_builder* sb, integer n, bool ordinal) {
    if (n < 20)
        string_builder_append(sb, get_small_name(&small[n], ordinal));
    else if (n < 100) {
        if (n % 10 == 0) {
            string_builder_append(sb, get_small_name(&tens[n/10 - 2], ordinal));
        } else {
            string_builder_append(sb, get_small_name(&tens[n/10 - 2], false));
            string_builder_append(sb, "-");
            string_builder_append(sb, get_small_name(&small[n % 10], ordinal));
        }
    } else {
        const size_t names_len = sizeof(named_numbers)/sizeof(named_numbers[0]);
        for (size_t i = 1; i <= names_len; ++i) {
            if (i == names_len || n < named_numbers[i].number) {
                integer p = named_numbers[i-1].number;
                append_number_name(sb, n/p, false);
                string_builder_append(sb, " ");
                if (n % p == 0) {
                    string_builder_append(sb, get_big_name(&named_numbers[i-1], ordinal));
                } else {
                    string_builder_append(sb, get_big_name(&named_numbers[i-1], false));
                    string_builder_append(sb, " ");
                    append_number_name(sb, n % p, ordinal);
                }
                break;
            }
        }
    }
}

char* number_name(integer n, bool ordinal) {
    string_builder result = { 0 };
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
