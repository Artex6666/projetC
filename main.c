#include <stdio.h>
#include <string.h>
#include "functions/back.h"
#include "functions/front.h"  // pour start_motus_front()

int main(int argc, char* argv[]) {

    if (argc > 1 && strcmp(argv[1], "--sdl") == 0) {
        // Lancer la version SDL
        return start_motus_front();
    } else {
        // Lancer la version console “classique”
        char* secret_word = get_random_word();
        if (!secret_word) {
            printf("Impossible de récupérer un mot.\n");
            return 1;
        }
        printf("Mot secret (DEBUG) : %s\n", secret_word);

        char guess[100];
        printf("Entrez un mot : ");
        scanf("%99s", guess);

        if (!is_valid_word(guess)) {
            printf("Le mot \"%s\" n'est pas dans la liste.\n", guess);
        } else {
            int length = strlen(secret_word);
            if (strlen(guess) != (size_t)length) {
                printf("Le mot proposé n'a pas la même longueur.\n");
            } else {
                int* result = malloc(length * sizeof(int));
                if (!result) {
                    perror("Erreur d'allocation mémoire.");
                    free(secret_word);
                    return 1;
                }

                check_word(guess, secret_word, result);

                printf("Résultat : ");
                for (int i = 0; i < length; i++) {
                    printf("%d ", result[i]);
                }
                printf("\n");

                free(result);
            }
        }
        free(secret_word);
        return 0;
    }
}
