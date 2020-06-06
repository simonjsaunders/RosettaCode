#include "bit_array.h"
#include <assert.h>
#include <stdlib.h>

bool bit_array_create(bit_array* b, uint32_t size) {
    uint32_t* array = calloc((size + 31)/32, sizeof(uint32_t));
    if (array == NULL)
        return false;
    b->size = size;
    b->array = array;
    return true;
}

void bit_array_destroy(bit_array* b) {
    free(b->array);
    b->array = NULL;
}

void bit_array_set(bit_array* b, uint32_t index, bool value) {
    assert(index < b->size);
    uint32_t* p = &b->array[index >> 5];
    uint32_t bit = 1 << (index & 31);
    if (value)
        *p |= bit;
    else
        *p &= ~bit;
}

bool bit_array_get(const bit_array* b, uint32_t index) {
    assert(index < b->size);
    uint32_t* p = &b->array[index >> 5];
    uint32_t bit = 1 << (index & 31);
    return (*p & bit) != 0;
}
