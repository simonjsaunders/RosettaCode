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

const named_number* get_named_number(integer n) {
    const size_t names_len = sizeof(named_numbers)/sizeof(named_numbers[0]);
    for (size_t i = 0; i + 1 < names_len; ++i) {
        if (n < named_numbers[i + 1].number)
            return &named_numbers[i];
    }
    return &named_numbers[names_len - 1];
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

typedef struct string_buffer_tag {
    size_t size;
    size_t capacity;
    char* string;
} string_buffer;

void string_buffer_create(string_buffer* buffer, size_t capacity) {
    buffer->size = 0;
    buffer->capacity = capacity;
    buffer->string = xmalloc(capacity);
}

void string_buffer_destroy(string_buffer* buffer) {
    free(buffer->string);
    buffer->string = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

void string_buffer_clear(string_buffer* buffer) {
    buffer->size = 0;
}

void string_buffer_append(string_buffer* buffer, const char* str, size_t len) {
    size_t min_capacity = buffer->size + len + 1;
    if (buffer->capacity < min_capacity) {
        size_t new_capacity = 5*buffer->capacity/4;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        buffer->string = xrealloc(buffer->string, new_capacity);
        buffer->capacity = new_capacity;
    }
    memcpy(buffer->string + buffer->size, str, len + 1);
    buffer->size += len;
}

typedef struct word_tag {
    size_t offset;
    size_t length;
} word;

typedef struct word_list_tag {
    size_t size;
    size_t capacity;
    word* words;
    string_buffer str;
} word_list;

void word_list_create(word_list* words, size_t capacity) {
    words->size = 0;
    words->capacity = capacity;
    words->words = xmalloc(capacity * sizeof(word));
    string_buffer_create(&words->str, capacity*8);
}

void word_list_destroy(word_list* words) {
    string_buffer_destroy(&words->str);
    free(words->words);
    words->words = NULL;
    words->size = 0;
    words->capacity = 0;
}

void word_list_clear(word_list* words) {
    string_buffer_clear(&words->str);
    words->size = 0;
}

void word_list_append(word_list* words, const char* str) {
    size_t offset = words->str.size;
    size_t len = strlen(str);
    string_buffer_append(&words->str, str, len);
    size_t min_capacity = words->size + 1;
    if (words->capacity < min_capacity) {
        size_t new_capacity = (words->capacity * 5)/4;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        words->words = xrealloc(words->words, new_capacity * sizeof(word));
        words->capacity = new_capacity;
    }
    word* w = &words->words[words->size++];
    w->offset = offset;
    w->length = len;
}

void word_list_extend(word_list* words, const char* str) {
    word* w = &words->words[words->size - 1];
    size_t len = strlen(str);
    w->length += len;
    string_buffer_append(&words->str, str, len);
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
            word_list_append(words, get_small_name(&tens[n/10 - 2], false));
            word_list_extend(words, "-");
            word_list_extend(words, get_small_name(&small[n % 10], ordinal));
        }
        count = 1;
    } else {
        const named_number* num = get_named_number(n);
        integer p = num->number;
        count += append_number_name(words, n/p, false);
        if (n % p == 0) {
            word_list_append(words, get_big_name(num, ordinal));
            ++count;
        } else {
            word_list_append(words, get_big_name(num, false));
            ++count;
            count += append_number_name(words, n % p, ordinal);
        }
    }
    return count;
}

size_t count_letters(const word_list* words, size_t index) {
    const word* w = &words->words[index];
    size_t letters = 0;
    const char* s = words->str.string + w->offset;
    for (size_t i = 0, n = w->length; i < n; ++i) {
        if (isalpha((unsigned char)s[i]))
            ++letters;
    }
    return letters;
}

void sentence(word_list* result, size_t count) {
    static const char* words[] = {
        "Four", "is", "the", "number", "of", "letters", "in", "the",
        "first", "word", "of", "this", "sentence,"
    };
    word_list_clear(result);
    size_t n = sizeof(words)/sizeof(words[0]);
    for (size_t i = 0; i < n; ++i)
        word_list_append(result, words[i]);
    for (size_t i = 1; count > n; ++i) {
        n += append_number_name(result, count_letters(result, i), false);
        word_list_append(result, "in");
        word_list_append(result, "the");
        n += 2;
        n += append_number_name(result, i + 1, true);
        // Append a comma to the final word
        word_list_extend(result, ",");
    }
}

size_t sentence_length(const word_list* words) {
    size_t n = words->size;
    if (n == 0)
        return 0;
    return words->str.size + n - 1;
}

int main() {
    setlocale(LC_ALL, "");
    size_t n = 201;
    word_list result = { 0 };
    word_list_create(&result, 1024);
    sentence(&result, n);
    printf("Number of letters in first %'lu words in the sequence:\n", n);
    for (size_t i = 0; i < n; ++i) {
        if (i != 0)
            printf("%c", i % 25 == 0 ? '\n' : ' ');
        printf("%'2lu", count_letters(&result, i));
    }
    printf("\nSentence length: %'lu\n", sentence_length(&result));
    for (n = 1000; n <= 10000000; n *= 10) {
        sentence(&result, n);
        const word* w = &result.words[n - 1];
        const char* s = result.str.string + w->offset;
        printf("The %'luth word is '%.*s' and has %lu letters. ", n, (int)w->length, s,
               count_letters(&result, n - 1));
        printf("Sentence length: %'lu\n" , sentence_length(&result));
    }
    word_list_destroy(&result);
    return 0;
}
