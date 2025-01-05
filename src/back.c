#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

#define MAX_WORD_LENGTH 100
#define FILE_PATH "../assets/words/liste_mots.txt"

static int calculate_difficulty(const char* word) {
    //printf("Calcul de la difficulté pour le mot : %s\n", word);
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
    //printf("Difficulté calculée : %d (longueur : %d, lettres distinctes : %d, voyelles : %d, consonnes : %d)\n",
    //       difficulty, length, distinct_count, voyelles, consonnes);
    return difficulty;
}

char* get_random_word(const char* difficulty) {
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    char** words = malloc(10000 * sizeof(char*));
    int word_count = 0;

    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; 
        int difficulty_score = calculate_difficulty(line);

        if ((strcmp(difficulty, "easy") == 0 && difficulty_score <= 10) ||
            (strcmp(difficulty, "medium") == 0 && difficulty_score > 10 && difficulty_score <= 20) ||
            (strcmp(difficulty, "hard") == 0 && difficulty_score > 20)) {
            words[word_count] = malloc(MAX_WORD_LENGTH);
            strcpy(words[word_count], line);
            word_count++;
        }
    }

    if (word_count == 0) {
        fclose(file);
        free(words);
        printf("Aucun mot correspondant à la difficulté choisie.\n");
        return NULL;
    }

    srand(time(NULL) + GetCurrentProcessId());
    int random_index;

    while (1) {
        
        random_index = rand() % word_count;

        printf("random_index: %d\n",random_index);
        printf("word_count: %d\n",word_count);
        char* random_word = words[random_index];  

        int difficulty_score = calculate_difficulty(random_word);

        printf("%d",difficulty_score);
        return random_word;  
    }

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