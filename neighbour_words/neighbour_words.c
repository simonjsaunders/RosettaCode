#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 80
#define MIN_LENGTH 9
#define WORD_SIZE (MIN_LENGTH + 1)
#define BUFFER_SIZE (MIN_LENGTH + 1)

int word_compare(const char* w1, const char* w2) {
    return memcmp(w1, w2, WORD_SIZE);
}

struct word_buffer {
    size_t start;
    size_t end;
    char words[BUFFER_SIZE][WORD_SIZE];
};

void word_buffer_init(struct word_buffer* buffer) {
    buffer->start = 0;
    buffer->end = 0;
}

void word_buffer_append(struct word_buffer* buffer, const char* word) {
    memcpy(buffer->words[buffer->end], word, WORD_SIZE);
    buffer->end = (buffer->end + 1) % BUFFER_SIZE;
    if (buffer->start == buffer->end)
        buffer->start = (buffer->start + 1) % BUFFER_SIZE;
}

size_t word_buffer_size(const struct word_buffer* buffer) {
    return (BUFFER_SIZE + buffer->end - buffer->start) % BUFFER_SIZE;
}

const char* word_buffer_get(const struct word_buffer* buffer, size_t index) {
    return buffer->words[(index + buffer->start) % BUFFER_SIZE];
}

bool word_buffer_contains(const struct word_buffer* buffer, const char* word) {
    const size_t n = word_buffer_size(buffer);
    for (size_t i = 0; i < n; ++i)
        if (word_compare(word, word_buffer_get(buffer, i)) == 0)
            return true;
    return false;
}

// The input file must consist of one word per line in alphabetical order.
int main(int argc, char** argv) {
    const char* filename = argc < 2 ? "unixdict.txt" : argv[1];
    FILE* in = fopen(filename, "r");
    if (!in) {
        perror(filename);
        return EXIT_FAILURE;
    }
    char line[MAX_WORD_SIZE];
    struct word_buffer words;
    word_buffer_init(&words);
    char prev_word[WORD_SIZE] = { 0 };
    int count = 0;
    while (fgets(line, sizeof(line), in)) {
        size_t len = strlen(line) - 1; // last character is newline
        if (len < MIN_LENGTH)
            continue;
        line[len] = '\0';
        word_buffer_append(&words, line);
        if (word_buffer_size(&words) < MIN_LENGTH)
            continue;
        char word[WORD_SIZE] = { 0 };
        for (size_t i = 0; i < MIN_LENGTH; ++i)
            word[i] = word_buffer_get(&words, i)[i];
        if (word_compare(word, prev_word) == 0)
            continue;
        if (word_buffer_contains(&words, word))
            printf("%2d. %s\n", ++count, word);
        memcpy(prev_word, word, WORD_SIZE);
    }
    fclose(in);
    return EXIT_SUCCESS;
}
