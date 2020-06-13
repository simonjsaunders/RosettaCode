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

void destroy_string(gpointer key) {
    g_string_free((GString*)key, TRUE);
}

int string_compare(gconstpointer p1, gconstpointer p2) {
    GString** s1 = (GString**)p1;
    GString** s2 = (GString**)p2;
    return strcmp((*s1)->str, (*s2)->str);
}

GPtrArray* load_dictionary(const char* file) {
    FILE* in = fopen(file, "r");
    if (in == 0) {
        perror(file);
        return NULL;
    }
    GPtrArray* dict = g_ptr_array_new_full(1024, destroy_string);
    GString* line = g_string_sized_new(64);
    while (get_line(in, line))
        g_ptr_array_add(dict, g_string_new(line->str));
    g_ptr_array_sort(dict, string_compare);
    g_string_free(line, TRUE);
    return dict;
}

void rotate(char* str, size_t len) {
    char c = str[0];
    memmove(str, str + 1, len - 1);
    str[len - 1] = c;
}

bool dictionary_search(const GPtrArray* dictionary, const GString* word) {
    return bsearch(&word, dictionary->pdata, dictionary->len, sizeof(GString*),
                   string_compare) != NULL;
}

void find_teacup_words(GPtrArray* dictionary) {
    GHashTable* found = g_hash_table_new_full((GHashFunc)g_string_hash,
                                              (GEqualFunc)g_string_equal,
                                              destroy_string, NULL);
    GPtrArray* teacup_words = g_ptr_array_new_full(8, destroy_string);
    for (size_t i = 0, n = dictionary->len; i < n; ++i) {
        GString* word = g_ptr_array_index(dictionary, i);
        size_t len = word->len;
        if (len < 3 || g_hash_table_contains(found, word))
            continue;
        g_ptr_array_set_size(teacup_words, 0);
        GString* temp = g_string_new(word->str);
        for (size_t i = 0; i < len - 1; ++i) {
            rotate(temp->str, len);
            if (g_string_equal(word, temp)
                || !dictionary_search(dictionary, temp))
                break;
            g_ptr_array_add(teacup_words, g_string_new(temp->str));
        }
        g_string_free(temp, TRUE);
        if (teacup_words->len == len - 1) {
            printf("%s", word->str);
            g_hash_table_insert(found, g_string_new(word->str), NULL);
            for (size_t i = 0; i < len - 1; ++i) {
                GString* teacup_word = g_ptr_array_index(teacup_words, i);
                printf(" %s", teacup_word->str);
                g_hash_table_insert(found, g_string_new(teacup_word->str), NULL);
            }
            printf("\n");
        }
    }
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
