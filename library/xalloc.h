#ifndef XALLOC_H
#define XALLOC_H

#include <stddef.h>

void fatal(const char* message);
void* xmalloc(size_t bytes);
void* xcalloc(size_t count, size_t size);
void* xrealloc(void* p, size_t bytes);
char* xstrdup(const char* str);

#endif
