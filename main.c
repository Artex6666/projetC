#include <stdio.h>
#include <stdlib.h>
#include "functions/back.h"

int main() {
    const char* difficulty = "hard"; 
    char* word = get_random_word(difficulty); 
    if (word != NULL) {
        printf("Mot aléatoire (difficulté %s) : %s\n", difficulty, word);
        free(word);
    } else {
        printf("Aucun mot trouvé pour cette difficulté.\n");
    }
    return 0;
}
