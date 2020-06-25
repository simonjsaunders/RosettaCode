#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

bool get_line(FILE* in, GString* line) {
    int c, count = 0;
    g_string_set_size(line, 0);
    while ((c = getc(in)) != EOF) {
        ++count;
        if (c == '\n')
            break;
        g_string_append_c(line, c);
    }
    return count > 0;
}

int string_compare(gconstpointer p1, gconstpointer p2) {
    const char* const* s1 = p1;
    const char* const* s2 = p2;
    return strcmp(*s1, *s2);
}

GPtrArray* load_dictionary(const char* file) {
    FILE* in = fopen(file, "r");
    if (in == 0) {
        perror(file);
        return NULL;
    }
    GPtrArray* dict = g_ptr_array_new_full(1024, g_free);
    GString* line = g_string_sized_new(64);
    while (get_line(in, line))
        g_ptr_array_add(dict, g_strdup(line->str));
    g_ptr_array_sort(dict, string_compare);
    g_string_free(line, TRUE);
    fclose(in);
    return dict;
}

void rotate(char* str, size_t len) {
    char c = str[0];
    memmove(str, str + 1, len - 1);
    str[len - 1] = c;
}

bool dictionary_search(const GPtrArray* dictionary, const char* word) {
    return bsearch(&word, dictionary->pdata, dictionary->len, sizeof(char*),
                   string_compare) != NULL;
}

void find_teacup_words(GPtrArray* dictionary) {
    GHashTable* found = g_hash_table_new_full(g_str_hash, g_str_equal,
                                              g_free, NULL);
    GPtrArray* teacup_words = g_ptr_array_new_full(8, g_free);
    GString* temp = g_string_sized_new(8);
    for (size_t i = 0, n = dictionary->len; i < n; ++i) {
        const char* word = g_ptr_array_index(dictionary, i);
        size_t len = strlen(word);
        if (len < 3 || g_hash_table_contains(found, word))
            continue;
        g_ptr_array_set_size(teacup_words, 0);
        g_string_assign(temp, word);
        for (size_t i = 0; i < len - 1; ++i) {
            rotate(temp->str, len);
            if (strcmp(word, temp->str) == 0
                || !dictionary_search(dictionary, temp->str))
                break;
            g_ptr_array_add(teacup_words, g_strdup(temp->str));
        }
        if (teacup_words->len == len - 1) {
            printf("%s", word);
            g_hash_table_add(found, g_strdup(word));
            for (size_t i = 0; i < len - 1; ++i) {
                const char* teacup_word = g_ptr_array_index(teacup_words, i);
                printf(" %s", teacup_word);
                g_hash_table_add(found, g_strdup(teacup_word));
            }
            printf("\n");
        }
    }
    g_string_free(temp, TRUE);
    g_ptr_array_free(teacup_words, TRUE);
    g_hash_table_destroy(found);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s dictionary\n", argv[0]);
        return EXIT_FAILURE;
    }
    GPtrArray* dictionary = load_dictionary(argv[1]);
    if (dictionary == NULL)
        return EXIT_FAILURE;
    find_teacup_words(dictionary);
    g_ptr_array_free(dictionary, TRUE);
    return EXIT_SUCCESS;
}
