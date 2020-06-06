#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct bit_array_tag {
    uint32_t size;
    uint32_t* array;
} bit_array;

bool bit_array_create(bit_array* b, uint32_t size);
void bit_array_destroy(bit_array* b);
void bit_array_set(bit_array* b, uint32_t index, bool value);
bool bit_array_get(const bit_array* b, uint32_t index);

#endif
