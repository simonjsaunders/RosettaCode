#include <stdio.h>

#define MAX_WORD 80

void word_wheel(const char* letters, char central, int min_length, FILE* dict) {
    int max_count[26] = { 0 };
    for (const char* p = letters; *p; ++p) {
        char c = *p;
        if (c >= 'a' && c <= 'z')
            ++max_count[c - 'a'];
    }
    char word[MAX_WORD + 1] = { 0 };
    while (fgets(word, MAX_WORD, dict)) {
        int count[26] = { 0 };
        for (const char* p = word; *p; ++p) {
            char c = *p;
            if (c == '\n') {
                if (p >= word + min_length && count[central - 'a'] > 0)
                    printf("%s", word);
            } else if (c >= 'a' && c <= 'z') {
                int i = c - 'a';
                if (++count[i] > max_count[i]) {
                    break;
                }
            } else {
                break;
            }
        }
    }
}

int main(int argc, char** argv) {
    const char* dict = argc == 2 ? argv[1] : "unixdict.txt";
    FILE* in = fopen(dict, "r");
    if (in == NULL) {
        perror(dict);
        return 1;
    }
    word_wheel("ndeokgelw", 'k', 3, in);
    fclose(in);
    return 0;
}
