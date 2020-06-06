#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_buffer.h"
#include "xalloc.h"

typedef struct hash_table_entry_tag {
    char* key;
    int value;
    size_t hash;
} hash_table_entry;

typedef struct hash_table_tag {
    size_t entries;
    size_t size;
    hash_table_entry* table;
} hash_table;

void hash_table_create(hash_table* h, size_t size) {
    hash_table_entry* table = xcalloc(size, sizeof(hash_table_entry));
    h->size = size;
    h->entries = 0;
    h->table = table;
}

void hash_table_destroy(hash_table* h) {
    for (size_t i = 0; i < h->size; ++i)
        free(h->table[i].key);
    free(h->table);
    h->table = NULL;
}

size_t hash(const char* str) {
    size_t h = 5381;
    for (; *str; ++str)
        h = ((h << 5) + h) + *str;
    return h;
}

hash_table_entry* hash_table_find_entry(hash_table* ht, const char* key) {
    size_t i = hash(key) % ht->size;
    while (ht->table[i].key != NULL
           && strcmp(ht->table[i].key, key) != 0) {
        i = (i + 1) % ht->size;
    }
    return &ht->table[i];
}

void hash_table_grow(hash_table* ht) {
    size_t new_size = ht->size * 2;
    hash_table_entry* new_table = xcalloc(new_size, sizeof(hash_table_entry));
    for (size_t i = 0; i < ht->size; ++i) {
        hash_table_entry* e = &ht->table[i];
        if (e->key == NULL)
            continue;
        size_t j = e->hash % new_size;
        while (new_table[j].key != NULL)
            j = (j + 1) % new_size;
        hash_table_entry* new_entry = &new_table[j];
        new_entry->key = e->key;
        new_entry->hash = e->hash;
        new_entry->value = e->value;
    }
    free(ht->table);
    ht->table = new_table;
    ht->size = new_size;
}

void hash_table_inc(hash_table* ht, const char* str) {
    hash_table_entry* e = hash_table_find_entry(ht, str);
    if (e->key != NULL) {
        ++e->value;
        return;
    }
    // 75% load factor
    if (4 * ht->entries >= 3 * ht->size) {
        hash_table_grow(ht);
        e = hash_table_find_entry(ht, str);
    }
    assert(e->key == NULL);
    e->key = xstrdup(str);
    e->value = 1;
    e->hash = hash(str);
    ++ht->entries;
}

int hash_table_entry_cmp(const void* p1, const void* p2) {
    const hash_table_entry* e1 = (const hash_table_entry*)p1;
    const hash_table_entry* e2 = (const hash_table_entry*)p2;
    if (e1->value > e2->value)
        return -1;
    if (e1->value < e2->value)
        return 1;
    return strcmp(e1->key, e2->key);
}

// A word is defined to be any run of characters for which isalpha
// returns true, i.e. upper or lower case letters. All words are
// converted to lower case.
bool get_word(FILE* in, string_buffer* buffer) {
    int c;
    string_buffer_clear(buffer);
    while ((c = getc(in)) != EOF) {
        if (isalpha(c)) {
            string_buffer_append(buffer, tolower(c));
            break;
        }
    }
    if (buffer->size == 0)
        return false;
    while ((c = getc(in)) != EOF) {
        if (!isalpha(c))
            break;
        string_buffer_append(buffer, tolower(c));
    }
    return true;
}

void get_top_words(FILE* in, int count) {
    hash_table ht = { 0 };
    hash_table_create(&ht, 1024);
    // Store word counts in the hash table
    string_buffer buffer;
    string_buffer_create(&buffer, 64);
    while (get_word(in, &buffer))
        hash_table_inc(&ht, buffer.string);
    string_buffer_destroy(&buffer);
    // Sort words in decreasing order of frequency
    hash_table_entry* words = xmalloc(sizeof(hash_table_entry) * ht.entries);
    for (size_t i = 0, j = 0; i < ht.size; ++i) {
        if (ht.table[i].key != NULL)
            words[j++] = ht.table[i];
    }
    qsort(words, ht.entries, sizeof(hash_table_entry), hash_table_entry_cmp);
    if (count > ht.entries)
        count = ht.entries;
    // Print the most common words
    printf("Top %d words\n", count);
    printf("Rank\tCount\tWord\n");
    for (int i = 0; i < count; ++i)
        printf("%d\t%d\t%s\n", i + 1, words[i].value, words[i].key);
    free(words);
    hash_table_destroy(&ht);
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
