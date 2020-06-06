#include "string_buffer.h"
#include "xalloc.h"
#include <stdlib.h>
#include <string.h>

void string_buffer_create(string_buffer* buffer, size_t capacity) {
    buffer->size = 0;
    buffer->capacity = capacity;
    buffer->string = xmalloc(capacity);
}

void string_buffer_destroy(string_buffer* buffer) {
    free(buffer->string);
    buffer->string = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

void string_buffer_clear(string_buffer* buffer) {
    buffer->size = 0;
    buffer->string[0] = 0;
}

void string_buffer_append(string_buffer* buffer, char ch) {
    size_t min_capacity = buffer->size + 2;
    if (buffer->capacity < min_capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        buffer->string = xrealloc(buffer->string, new_capacity);
        buffer->capacity = new_capacity;
    }
    buffer->string[buffer->size++] = ch;
    buffer->string[buffer->size] = 0;
}

void string_buffer_append_str(string_buffer* buffer, const char* str) {
    size_t len = strlen(str);
    size_t min_capacity = buffer->size + len + 1;
    if (buffer->capacity < min_capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < min_capacity)
            new_capacity = min_capacity;
        buffer->string = xrealloc(buffer->string, new_capacity);
        buffer->capacity = new_capacity;
    }
    memcpy(buffer->string + buffer->size, str, len + 1);
    buffer->size += len;
}
