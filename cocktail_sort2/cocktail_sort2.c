#include <stdio.h>
#include <string.h>

void swap(char* p1, char* p2, size_t size) {
    char tmp[size];
    memcpy(tmp, p1, size);
    memmove(p1, p2, size);
    memcpy(p2, tmp, size);
}

void cocktail_shaker_sort(void* base, size_t count, size_t size,
                          int (*cmp)(const void*, const void*)) {
    size_t begin = 0;
    size_t end = count;
    if (end == 0)
        return;
    char* array = base;
    for (--end; begin < end; ) {
        size_t new_begin = end;
        size_t new_end = begin;
        for (size_t i = begin; i < end; ++i) {
            char* p1 = array + i * size;
            char* p2 = p1 + size;
            if (cmp(p1, p2) > 0) {
                swap(p1, p2, size);
                new_end = i;
            }
        }
        end = new_end;
        for (size_t i = end; i > begin; --i) {
            char* p1 = array + (i - 1) * size;
            char* p2 = p1 + size;
            if (cmp(p1, p2) > 0) {
                swap(p1, p2, size);
                new_begin = i;
            }
        }
        begin = new_begin;
    }
}

int string_compare(const void* p1, const void* p2) {
    const char* const* s1 = p1;
    const char* const* s2 = p2;
    return strcmp(*s1, *s2);
}

void print(const char** a, size_t len) {
    for (size_t i = 0; i < len; ++i)
        printf("%s ", a[i]);
    printf("\n");
}

int main() {
    const char* a[] = { "one", "two", "three", "four", "five",
        "six", "seven", "eight" };
    const size_t len = sizeof(a)/sizeof(a[0]);
    printf("before: ");
    print(a, len);
    cocktail_shaker_sort(a, len, sizeof(char*), string_compare);
    printf("after: ");
    print(a, len);
    return 0;
}
