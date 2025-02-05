#include "../include/back/word.h"
#include "../include/back/difficulty.h"

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

        if ((strcmp(difficulty, "easy") == 0 && difficulty_score <= 12) ||
            (strcmp(difficulty, "medium") == 0 && difficulty_score > 12 && difficulty_score <= 17) ||
            (strcmp(difficulty, "hard") == 0 && difficulty_score > 17)) {
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
        char* random_word = words[random_index];
        int difficulty_score = calculate_difficulty(random_word);

        if ((strcmp(difficulty, "easy") == 0 && difficulty_score <= 10) ||
            (strcmp(difficulty, "medium") == 0 && difficulty_score > 10 && difficulty_score <= 20) ||
            (strcmp(difficulty, "hard") == 0 && difficulty_score > 20)) {
            return random_word;
        }
    }
}

int is_valid_word(const char* word) {
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 0;
    }

    char upper_word[MAX_WORD_LENGTH];
    toUpperCase(word, upper_word, MAX_WORD_LENGTH);

    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(upper_word, line) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

char check_word(const char* guess, const char* secret, int* result) {
    int length = strlen(secret);
    int used_positions[length];
    char valid_word = 1;
    memset(used_positions, 0, sizeof(used_positions));

    char upper_guess[MAX_WORD_LENGTH];
    toUpperCase(guess, upper_guess, MAX_WORD_LENGTH);

    for (int i = 0; i < length; i++) {
        if (upper_guess[i] == secret[i]) {
            result[i] = 2;
            used_positions[i] = 1;
        } else {
            result[i] = 0;
            valid_word = 0;
        }
    }

    for (int i = 0; i < length; i++) {
        if (result[i] == 2) continue;

        for (int j = 0; j < length; j++) {
            if (!used_positions[j] && upper_guess[i] == secret[j]) {
                result[i] = 1;
                used_positions[j] = 1;
                valid_word = 0;
                break;
            }
        }
    }
    return valid_word;
}