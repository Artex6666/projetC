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
