#include <stdio.h>
#include <stdlib.h>

int mertens_number(int n, int* cache, int max) {
    if (n < 0 || n > max)
        return 0;
    int* c = &cache[2 * n];
    if (c[0] == 1)
        return c[1];
    int m = 1;
    for (int k = 2; k <= n; ++k)
        m -= mertens_number(n/k, cache, max);
    c[0] = 1;
    c[1] = m;
    return m;
}

int main() {
    const int max = 1000;
    int* cache = calloc(2 * (max+1), sizeof(int));
    if (cache == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    printf("First 199 Mertens numbers:\n");
    const int count = 200;
    for (int i = 0, column = 0; i < count; ++i) {
        if (column > 0)
            printf(" ");
        if (i == 0)
            printf("  ");
        else
            printf("%2d", mertens_number(i, cache, max));
        ++column;
        if (column == 20) {
            printf("\n");
            column = 0;
        }
    }
    int zero = 0, cross = 0, previous = 0;
    for (int i = 1; i <= max; ++i) {
        int m = mertens_number(i, cache, max);
        if (m == 0) {
            ++zero;
            if (previous != 0)
                ++cross;
        }
        previous = m;
    }
    free(cache);
    printf("M(n) is zero %d times for 1 <= n <= %d.\n", zero, max);
    printf("M(n) crosses zero %d times for 1 <= n <= %d.\n", cross, max);
    return 0;
}
