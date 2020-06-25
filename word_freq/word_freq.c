#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <glib.h>

// A word is defined to be any run of characters for which isalpha
// returns true, i.e. upper or lower case letters. All words are
// converted to lower case.
bool get_word(FILE* in, GString* word) {
    int c;
    g_string_set_size(word, 0);
    while ((c = getc(in)) != EOF) {
        if (isalpha(c)) {
            g_string_append_c(word, tolower(c));
            break;
        }
    }
    if (word->len == 0)
        return false;
    while ((c = getc(in)) != EOF) {
        if (!isalpha(c))
            break;
        g_string_append_c(word, tolower(c));
    }
    return true;
}

typedef struct word_count_tag {
    const char* word;
    size_t count;
} word_count;

int compare_word_count(const void* p1, const void* p2) {
    const word_count* w1 = p1;
    const word_count* w2 = p2;
    if (w1->count > w2->count)
        return -1;
    if (w1->count < w2->count)
        return 1;
    return 0;
}

void get_top_words(FILE* in, size_t count) {
    GHashTable* ht = g_hash_table_new_full(g_str_hash, g_str_equal,
                                           g_free, g_free);
    // Store word counts in the hash table
    GString* word = g_string_sized_new(64);
    while (get_word(in, word)) {
        gpointer value = g_hash_table_lookup(ht, word->str);
        if (value != NULL) {
            size_t* count = value;
            ++*count;
        } else {
            size_t* count = g_new(size_t, 1);
            *count = 1;
            g_hash_table_insert(ht, g_strdup(word->str), count);
        }
    }
    g_string_free(word, TRUE);

    // Sort words in decreasing order of frequency
    size_t size = g_hash_table_size(ht);
    word_count* words = g_new(word_count, size);
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, ht);
    for (size_t i = 0; g_hash_table_iter_next(&iter, &key, &value); ++i) {
        words[i].word = (char*)key;
        words[i].count = *(size_t*)value;
    }
    qsort(words, size, sizeof(word_count), compare_word_count);

    // Print the most common words
    if (count > size)
        count = size;
    printf("Top %lu words\n", count);
    printf("Rank\tCount\tWord\n");
    for (size_t i = 0; i < count; ++i)
        printf("%lu\t%lu\t%s\n", i + 1, words[i].count, words[i].word);
    g_free(words);
    g_hash_table_destroy(ht);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 1;
    }
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        perror(argv[1]);
        return 1;
    }
    get_top_words(in, 15);
    fclose(in);
    return 0;
}
