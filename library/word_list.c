#include "word_list.h"
#include "xalloc.h"
#include <stdlib.h>
#include <string.h>

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

static int word_compare(const void* p1, const void* p2) {
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
