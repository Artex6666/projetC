#include <stdio.h>
#include <stdlib.h>
#include "functions/back.h"

char* get_random_word();

int main() {
    char* word = get_random_word();
    if (word != NULL) {
        printf("Mot aléatoire : %s\n", word);
        free(word);
    }
    return 0;
}
