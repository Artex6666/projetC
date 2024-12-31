#include <stdio.h>
#include <stdlib.h>
#include "back.h"  // on inclut le header qui contient les prototypes

int main(void) {
    // 1) Tirage aléatoire d'un mot
    char* secret_word = get_random_word();
    if (secret_word == NULL) {
        printf("Impossible de récupérer un mot.\n");
        return 1;
    }
    printf("Mot secret (DEBUG) : %s\n", secret_word);

    // 2) Vérification d'un mot saisi par l'utilisateur
    char guess[100];
    printf("Entrez un mot : ");
    scanf("%99s", guess);

    // Vérifier si le mot proposé est valide (c’est-à-dire présent dans la liste)
    if (!is_valid_word(guess)) {
        printf("Le mot \"%s\" n'est pas dans la liste.\n", guess);
    } else {
        // 3) Vérification lettre par lettre
        // On suppose que le mot secret et le mot proposé sont de même longueur
        int length = strlen(secret_word);
        if (strlen(guess) != (size_t)length) {
            printf("Le mot proposé n'a pas la même longueur que le mot secret.\n");
        } else {
            int* result = malloc(length * sizeof(int));
            if (!result) {
                perror("Erreur d'allocation mémoire.");
                free(secret_word);
                return 1;
            }

            check_word(guess, secret_word, result);

            // 4) Affichage des résultats 
            // 0 = lettre absente, 1 = lettre présente mais mal placée, 2 = bien placée.
            printf("Résultat : ");
            for (int i = 0; i < length; i++) {
                printf("%d ", result[i]);
            }
            printf("\n");

            free(result);
        }
    }

    // Nettoyage
    free(secret_word);
    return 0;
}
