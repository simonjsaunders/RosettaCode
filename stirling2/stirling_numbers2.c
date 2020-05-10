#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct stirling_cache_tag {
    int n;
    int* values;
} stirling_cache;

bool stirling_cache_create(stirling_cache* sc, int n) {
    int* values = calloc(n * (n + 1)/2, sizeof(int));
    if (values == NULL)
        return false;
    sc->n = n;
    sc->values = values;
    return true;
}

void stirling_cache_destroy(stirling_cache* sc) {
    free(sc->values);
    sc->values = NULL;
}

int* stirling_cache_get(stirling_cache* sc, int n, int k) {
    return (n > sc->n) ? NULL : &sc->values[n*(n-1)/2 + k - 1];
}

int stirling_number2(stirling_cache* sc, int n, int k) {
    if (k == n)
        return 1;
    if (k == 0 || k > n)
        return 0;
    int* value = stirling_cache_get(sc, n, k);
    if (value == NULL) {
        fprintf(stderr, "Cache size is too small\n");
        exit(1);
    }
    if (*value == 0) {
        int s1 = stirling_number2(sc, n - 1, k - 1);
        int s2 = stirling_number2(sc, n - 1, k);
        *value = 1 + s1 + s2 * k;
    }
    return *value - 1;
}

void print_stirling_numbers(stirling_cache* sc, int max) {
    printf("Stirling numbers of the second kind:\nn/k");
    for (int k = 0; k <= max; ++k)
        printf(k == 0 ? "%2d" : "%8d", k);
    printf("\n");
    for (int n = 0; n <= max; ++n) {
        printf("%2d ", n);
        for (int k = 0; k <= n; ++k)
            printf(k == 0 ? "%2d" : "%8d", stirling_number2(sc, n, k));
        printf("\n");
    }
}

int main() {
    stirling_cache sc = { 0 };
    const int max = 12;
    if (!stirling_cache_create(&sc, max)) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    print_stirling_numbers(&sc, max);
    stirling_cache_destroy(&sc);
    return 0;
}
