#include "xalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fatal(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void* xmalloc(size_t bytes) {
    void* ptr = malloc(bytes);
    if (ptr == NULL)
        fatal("Out of memory");
    return ptr;
}

void* xcalloc(size_t count, size_t size) {
    void* ptr = calloc(count, size);
    if (ptr == NULL)
        fatal("Out of memory");
    return ptr;
}

void* xrealloc(void* p, size_t bytes) {
    void* ptr = realloc(p, bytes);
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
