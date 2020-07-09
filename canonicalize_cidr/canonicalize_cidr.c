#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef struct cidr_tag {
    uint32_t address;
    uint32_t mask;
} cidr_t;

// Convert a string in CIDR format to an IPv4 address and netmask,
// if possible. Also performs CIDR canonicalization.
bool cidr_parse(const char* str, cidr_t* cidr) {
    int a, b, c, d, m;
    if (sscanf(str, "%d.%d.%d.%d/%d", &a, &b, &c, &d, &m) != 5)
        return false;
    if (m < 1 || m > 32)
        return false;
    if (a < 0 || a > UINT8_MAX)
        return false;
    if (b < 0 || b > UINT8_MAX)
        return false;
    if (c < 0 || c > UINT8_MAX)
        return false;
    if (d < 0 || d > UINT8_MAX)
        return false;
    uint32_t mask = ~((1 << (32 - m)) - 1);
    uint32_t address = (a << 24) + (b << 16) + (c << 8) + d;
    address &= mask;
    cidr->address = address;
    cidr->mask = mask;
    return true;
}

uint8_t mask_length(uint32_t mask) {
    uint8_t length = 32;
    for (; (mask & 1) == 0 && length > 0; mask >>= 1, --length) {}
    return length;
}

// Write a string in CIDR notation into the supplied buffer.
void cidr_format(const cidr_t* cidr, char* str, size_t size) {
    uint8_t m = mask_length(cidr->mask);
    uint32_t address = cidr->address;
    uint8_t d = address & UINT8_MAX;
    address >>= 8;
    uint8_t c = address & UINT8_MAX;
    address >>= 8;
    uint8_t b = address & UINT8_MAX;
    address >>= 8;
    uint8_t a = address & UINT8_MAX;
    snprintf(str, size, "%hhu.%hhu.%hhu.%hhu/%hhu", a, b, c, d, m);
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        cidr_t cidr;
        if (cidr_parse(argv[i], &cidr)) {
            char out[32];
            cidr_format(&cidr, out, sizeof(out));
            puts(out);
        } else {
            fprintf(stderr, "%s: invalid CIDR\n", argv[i]);
        }
    }
    return 0;
}
