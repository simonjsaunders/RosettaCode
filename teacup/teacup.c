#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    words->size = 0;
    words->capacity = 0;
}

void word_list_clear(word_list* words) {
    for (size_t i = 0; i < words->size; ++i)
        free(words->words[i]);
    words->size = 0;
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

int word_compare(const void* p1, const void* p2) {
    const char* w1 = *(const char**)p1;
    const char* w2 = *(const char**)p2;
    return strcmp(w1, w2);
}

void word_list_sort(word_list* words) {
    qsort(words->words, words->size, sizeof(char*), word_compare);
}

// Binary seach. Assumes word list has been sorted.
bool word_list_bsearch(const word_list* words, const char* word) {
    return bsearch(&word, words->words, words->size, sizeof(char*), word_compare) != NULL;
}

// Linear search - OK if list is short.
bool word_list_contains(const word_list* words, const char* word) {
    for (size_t i = 0; i < words->size; ++i) {
        if (strcmp(word, words->words[i]) == 0)
            return true;
    }
    return false;
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
    buffer->string[0] = 0;
}

void string_buffer_append(string_buffer* buffer, char ch) {
    size_t min_capacity = buffer->size + 2;
    if (buffer->capacity < min_capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        buffer->string = xrealloc(buffer->string, new_capacity);
        buffer->capacity = new_capacity;
    }
    buffer->string[buffer->size++] = ch;
    buffer->string[buffer->size] = 0;
}

bool get_line(FILE* in, string_buffer* buffer) {
    int c, count = 0;
    string_buffer_clear(buffer);
    while ((c = getc(in)) != EOF) {
        ++count;
        if (c == '\n')
            break;
        string_buffer_append(buffer, c);
    }
    return count > 0;
}

bool load_dictionary(const char* file, word_list* words) {
    FILE* in = fopen(file, "r");
    if (in == 0) {
        perror(file);
        return false;
    }
    word_list_create(words, 1024);
    string_buffer buffer;
    string_buffer_create(&buffer, 64);
    while (get_line(in, &buffer))
        word_list_append(words, buffer.string);
    string_buffer_destroy(&buffer);
    word_list_sort(words);
    return true;
}

void rotate(char* str, size_t len) {
    char c = str[0];
    memmove(str, str + 1, len - 1);
    str[len - 1] = c;
}

void find_teacup_words(const word_list* words) {
    word_list teacup_words, found;
    word_list_create(&teacup_words, 8);
    word_list_create(&found, 8);
    for (size_t i = 0; i < words->size; ++i) {
        const char* word = words->words[i];
        size_t len = strlen(word);
        if (len < 3 || word_list_contains(&found, word))
            continue;
        word_list_clear(&teacup_words);
        char* copy = xstrdup(word);
        for (size_t i = 0; i < len - 1; ++i) {
            rotate(copy, len);
            if (strcmp(word, copy) == 0 || !word_list_bsearch(words, copy))
                break;
            word_list_append(&teacup_words, copy);
        }
        free(copy);
        if (teacup_words.size == len - 1) {
            printf("%s", word);
            word_list_append(&found, word);
            for (size_t i = 0; i < len - 1; ++i) {
                printf(" %s", teacup_words.words[i]);
                word_list_append(&found, teacup_words.words[i]);
            }
            printf("\n");
        }
    }
    word_list_destroy(&teacup_words);
    word_list_destroy(&found);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s dictionary\n", argv[0]);
        return EXIT_FAILURE;
    }
    word_list words;
    if (!load_dictionary(argv[1], &words))
        return EXIT_FAILURE;
    find_teacup_words(&words);
    word_list_destroy(&words);
    return EXIT_SUCCESS;
}
