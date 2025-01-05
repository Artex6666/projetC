#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 100
#define FILE_PATH "features/liste_mots.txt"

char* get_random_word() {
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    int word_count = 0;
    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        word_count++;
    }

    if (word_count == 0) {
        fclose(file);
        return NULL;
    }

    srand(time(NULL));
    int random_index = rand() % word_count;

    rewind(file);
    int current_index = 0;
    char* random_word = malloc(MAX_WORD_LENGTH);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_index == random_index) {
            line[strcspn(line, "\n")] = '\0'; 
            strcpy(random_word, line);
            break;
        }
        current_index++;
    }

    fclose(file);
    return random_word;
}

int is_valid_word(const char* word) {
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 0; 
    }

    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; 
        if (strcmp(word, line) == 0) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
}

void check_word(const char* guess, const char* secret, int* result) {
    int length = strlen(secret);
    int used_positions[length];
    memset(used_positions, 0, sizeof(used_positions)); 

    for (int i = 0; i < length; i++) {
        if (guess[i] == secret[i]) {
            result[i] = 2; 
            used_positions[i] = 1; 
        } else {
            result[i] = 0;
        }
    }

    for (int i = 0; i < length; i++) {
        if (result[i] == 2) continue; 

        for (int j = 0; j < length; j++) {
            if (!used_positions[j] && guess[i] == secret[j]) {
                result[i] = 1;
                used_positions[j] = 1; 
                break;
            }
        }
    }
}
