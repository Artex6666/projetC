#include "../include/back/difficulty.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int calculate_difficulty(const char* word) {
    int length = strlen(word);
    int distinct_letters[26] = {0};
    int voyelles = 0, consonnes = 0;

    for (int i = 0; i < length; i++) {
        char c = tolower(word[i]);
        if (isalpha(c)) {
            if (strchr("aeiou", c)) {
                voyelles++;
            } else {
                consonnes++;
            }
            distinct_letters[c - 'a'] = 1;
        }
    }

    int distinct_count = 0;
    for (int i = 0; i < 26; i++) {
        if (distinct_letters[i]) {
            distinct_count++;
        }
    }

    int difficulty = length + distinct_count - abs(voyelles - consonnes) / 2;
    return difficulty;
}

void toUpperCase(const char* input, char* output, int max_length) {
    int i = 0;
    while (input[i] != '\0' && i < max_length - 1) {
        output[i] = toupper((unsigned char)input[i]);
        i++;
    }
    output[i] = '\0';
}