#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stddef.h>

typedef struct string_buffer_tag {
    size_t size;
    size_t capacity;
    char* string;
} string_buffer;

void string_buffer_create(string_buffer* buffer, size_t capacity);
void string_buffer_destroy(string_buffer* buffer);
void string_buffer_clear(string_buffer* buffer);
void string_buffer_append(string_buffer* buffer, char ch);
void string_buffer_append_str(string_buffer* buffer, const char*);

#endif
