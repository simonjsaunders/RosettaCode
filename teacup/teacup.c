#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_buffer.h"
#include "word_list.h"
#include "xalloc.h"

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
