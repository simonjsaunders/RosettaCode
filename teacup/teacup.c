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

char* dictionary_search(const GPtrArray* dictionary, const char* word) {
    char** result = bsearch(&word, dictionary->pdata, dictionary->len,
                            sizeof(char*), string_compare);
    return result != NULL ? *result : NULL;
}

void find_teacup_words(GPtrArray* dictionary) {
    GHashTable* found = g_hash_table_new(g_str_hash, g_str_equal);
    GPtrArray* teacup_words = g_ptr_array_new();
    GString* temp = g_string_sized_new(8);
    for (size_t i = 0, n = dictionary->len; i < n; ++i) {
        char* word = g_ptr_array_index(dictionary, i);
        size_t len = strlen(word);
        if (len < 3 || g_hash_table_contains(found, word))
            continue;
        g_ptr_array_set_size(teacup_words, 0);
        g_string_assign(temp, word);
        bool is_teacup_word = true;
        for (size_t i = 0; i < len - 1; ++i) {
            rotate(temp->str, len);
            char* w = dictionary_search(dictionary, temp->str);
            if (w == NULL) {
                is_teacup_word = false;
                break;
            }
            if (strcmp(word, w) != 0 && !g_ptr_array_find(teacup_words, w, NULL))
                g_ptr_array_add(teacup_words, w);
        }
        if (is_teacup_word && teacup_words->len > 0) {
            printf("%s", word);
            g_hash_table_add(found, word);
            for (size_t i = 0; i < teacup_words->len; ++i) {
                char* teacup_word = g_ptr_array_index(teacup_words, i);
                printf(" %s", teacup_word);
                g_hash_table_add(found, teacup_word);
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
