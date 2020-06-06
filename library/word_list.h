#ifndef WORD_LIST_H
#define WORD_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct word_list_tag {
    size_t size;
    size_t capacity;
    char** words;
} word_list;

void word_list_create(word_list* words, size_t capacity);
void word_list_destroy(word_list* words);
void word_list_clear(word_list* words);
void word_list_append(word_list* words, const char* str);
void word_list_sort(word_list* words);

// Binary seach. Assumes word list has been sorted.
bool word_list_bsearch(const word_list* words, const char* word);

// Linear search - OK if list is short.
bool word_list_contains(const word_list* words, const char* word);

#endif
