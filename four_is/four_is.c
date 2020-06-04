#include <ctype.h>
#include <locale.h>
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

void fatal(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void* xmalloc(size_t n) {
    void* ptr = malloc(n);
    if (ptr == NULL)
        fatal("Out of memory");
    return ptr;
}

void* xrealloc(void* p, size_t n) {
    void* ptr = realloc(p, n);
    if (ptr == NULL)
        fatal("Out of memory");
    return ptr;
}

char* xstrdup(const char* str) {
    char* s = strdup(str);
    if (s == NULL)
        fatal("Out of memory");
    return s;
}

typedef struct word_list_tag {
    size_t size;
    size_t capacity;
    char** words;
} word_list;

void word_list_create(word_list* words, size_t capacity) {
    words->size = 0;
    words->capacity = capacity;
    words->words = xmalloc(capacity * sizeof(char*));
}

void word_list_destroy(word_list* words) {
    for (size_t i = 0; i < words->size; ++i)
        free(words->words[i]);
    free(words->words);
    words->words = NULL;
}

void word_list_append(word_list* words, const char* str) {
    size_t min_capacity = words->size + 1;
    if (words->capacity < min_capacity) {
        size_t new_capacity = (words->capacity * 3)/2;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        words->words = xrealloc(words->words, new_capacity * sizeof(char*));
        words->capacity = new_capacity;
    }
    words->words[words->size++] = xstrdup(str);
}

char* string_concat2(const char* str1, const char* str2) {
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    char* str = xmalloc(len1 + len2 + 1);
    memcpy(str, str1, len1);
    memcpy(str + len1, str2, len2);
    str[len1 + len2] = 0;
    return str;
}

char* string_concat3(const char* str1, const char* str2, const char* str3) {
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    const size_t len3 = strlen(str3);
    char* str = xmalloc(len1 + len2 + len3 + 1);
    memcpy(str, str1, len1);
    memcpy(str + len1, str2, len2);
    memcpy(str + len1 + len2, str3, len3);
    str[len1 + len2 + len3] = 0;
    return str;
}

size_t append_number_name(word_list* words, integer n, bool ordinal) {
    size_t count = 0;
    if (n < 20) {
        word_list_append(words, get_small_name(&small[n], ordinal));
        count = 1;
    } else if (n < 100) {
        if (n % 10 == 0) {
            word_list_append(words, get_small_name(&tens[n/10 - 2], ordinal));
        } else {
            const char* name1 = get_small_name(&tens[n/10 - 2], false);
            const char* name2 = get_small_name(&small[n % 10], ordinal);
            char* str = string_concat3(name1, "-", name2);
            word_list_append(words, str);
            free(str);
        }
        count = 1;
    } else {
        const size_t names_len = sizeof(named_numbers)/sizeof(named_numbers[0]);
        for (size_t i = 1; i <= names_len; ++i) {
            if (i == names_len || n < named_numbers[i].number) {
                integer p = named_numbers[i-1].number;
                count += append_number_name(words, n/p, false);
                if (n % p == 0) {
                    word_list_append(words, get_big_name(&named_numbers[i-1], ordinal));
                    ++count;
                } else {
                    word_list_append(words, get_big_name(&named_numbers[i-1], false));
                    ++count;
                    count += append_number_name(words, n % p, ordinal);
                }
                break;
            }
        }
    }
    return count;
}

size_t count_letters(const char* str) {
    size_t letters = 0;
    for (size_t i = 0, n = strlen(str); i < n; ++i) {
        if (isalpha((unsigned char)str[i]))
            ++letters;
    }
    return letters;
}

void sentence(word_list* result, size_t count) {
    static const char* words[] = {
        "Four", "is", "the", "number", "of", "letters", "in", "the",
        "first", "word", "of", "this", "sentence,"
    };
    word_list_create(result, count + 10);
    size_t n = sizeof(words)/sizeof(words[0]);
    for (size_t i = 0; i < n && i < count; ++i)
        word_list_append(result, words[i]);
    for (size_t i = 1; count > n; ++i) {
        n += append_number_name(result, count_letters(result->words[i]), false);
        word_list_append(result, "in");
        word_list_append(result, "the");
        n += 2;
        n += append_number_name(result, i + 1, true);
        // Append a comma to the final word
        char* last = result->words[result->size - 1];
        char* new_last = string_concat2(last, ",");
        free(last);
        result->words[result->size - 1] = new_last;
    }
}

size_t sentence_length(const word_list* words) {
    size_t n = words->size;
    if (n == 0)
        return 0;
    size_t length = n - 1;
    for (size_t i = 0; i < n; ++i)
        length += strlen(words->words[i]);
    return length;
}

int main() {
    setlocale(LC_ALL, "");
    size_t n = 201;
    word_list result = { 0 };
    sentence(&result, n);
    printf("Number of letters in first %'lu words in the sequence:\n", n);
    for (size_t i = 0; i < n; ++i) {
        if (i != 0)
            printf("%c", i % 25 == 0 ? '\n' : ' ');
        printf("%'2lu", count_letters(result.words[i]));
    }
    printf("\nSentence length: %'lu\n", sentence_length(&result));
    for (n = 1000; n <= 10000000; n *= 10) {
        word_list_destroy(&result);
        sentence(&result, n);
        const char* word = result.words[n - 1];
        printf("The %'luth word is '%s' and has %lu letters. ", n, word, count_letters(word));
        printf("Sentence length: %'lu\n" , sentence_length(&result));
    }
    word_list_destroy(&result);
    return 0;
}
