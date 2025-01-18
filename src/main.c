#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "../include/SDL/SDL.h"
#include "../include/SDL/SDL_ttf.h"
#include "../include/SDL/SDL_mixer.h"
#include "../include/SDL/SDL_blendmode.h"
#include <stdbool.h>
#include "../include/front/audio.h"
#include "../include/front/grid.h"
#include "../include/front/menu.h"
#include "../include/front/render.h"
#include "../include/front/timer.h"
#include <windows.h>
#include "../include/back/difficulty.h"
#include "../include/back/word.h"

#define SDL_MAIN_HANDLED

static const char* CHEMIN_POLICE = "./assets/fonts/FranceTVBrown-Regular.ttf";
static const char* MUSIQUE_FOND = "./assets/musics/background.wav";

// Définir les temps pour chaque difficulté (en millisecondes)
#define TEMPS_FACILE    (5 * 60 * 1000)  // 5 minutes
#define TEMPS_MOYEN     (2 * 60 * 1000)  // 2 minutes
#define TEMPS_DIFFICILE (1 * 60 * 1000)  // 1 minute

int start_motus_front(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window* fenetre = SDL_CreateWindow("Motus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!fenetre) {
        fprintf(stderr, "Erreur création fenêtre : %s\n", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger l'image de fond
    SDL_Surface* surface_fond = SDL_LoadBMP("assets/Images/Fondecran.bmp");
    if (!surface_fond) {
        fprintf(stderr, "Erreur chargement image de fond : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture_fond = SDL_CreateTextureFromSurface(renderer, surface_fond);
    SDL_FreeSurface(surface_fond);
    if (!texture_fond) {
        fprintf(stderr, "Erreur création texture de fond : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* police = TTF_OpenFont(CHEMIN_POLICE, 32);
    if (!police) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyTexture(texture_fond);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Afficher le menu de difficulté
    int temps_jeu = afficher_menu_difficulte(renderer, police);
    if (temps_jeu == -1) {
        // L'utilisateur a quitté le menu
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    Mix_Music* musique = Mix_LoadMUS(MUSIQUE_FOND);
    if (!musique) {
        fprintf(stderr, "Erreur chargement musique : %s\n", Mix_GetError());
        SDL_DestroyTexture(texture_fond);
        TTF_CloseFont(police);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    if (Mix_PlayMusic(musique, -1) == -1) {
        fprintf(stderr, "Erreur lecture musique : %s\n", Mix_GetError());
        SDL_DestroyTexture(texture_fond);
        Mix_FreeMusic(musique);
        TTF_CloseFont(police);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool actif = true;
    SDL_Event evenement;

    // Déterminer la difficulté en fonction du temps choisi
    const char* difficulte;
    if (temps_jeu == TEMPS_FACILE) {
        difficulte = "easy";
    } else if (temps_jeu == TEMPS_MOYEN) {
        difficulte = "medium";
    } else if (temps_jeu == TEMPS_DIFFICILE) {
        difficulte = "hard";
    } else {
        difficulte = "easy";  // Par défaut
    }
    
    // Obtenir un mot aléatoire en fonction de la difficulté
    char* mot_secret = get_random_word(difficulte);
    if (!mot_secret) {
        fprintf(stderr, "Impossible de récupérer un mot.\n");
        SDL_DestroyTexture(texture_fond);
        Mix_HaltMusic();
        Mix_FreeMusic(musique);
        TTF_CloseFont(police);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int longueur_mot = strlen(mot_secret);
    int lignes_grille = 6;
    int colonnes_grille = longueur_mot;

    // Allocation dynamique pour le tableau de résultats
    int** resultat = malloc(lignes_grille * sizeof(int*));
    for (int i = 0; i < lignes_grille; i++) {
        resultat[i] = malloc(colonnes_grille * sizeof(int));
        memset(resultat[i], 0, colonnes_grille * sizeof(int));
    }

    const char* propositions[6] = {NULL};
    int ligne_actuelle = 0;

    char* tentative = malloc((colonnes_grille + 1) * sizeof(char));
    memset(tentative, 0, (colonnes_grille + 1) * sizeof(char));
    int index_lettre = 0;

    Uint32 temps_debut = SDL_GetTicks();
    Uint32 temps_restant = temps_jeu;

    while (actif) {
       // Calcul du temps écoulé depuis le début (en millisecondes)
        Uint32 temps_ecoule = SDL_GetTicks() - temps_debut;

            // Si le temps écoulé dépasse ou égale le temps imparti, on arrête
        if (temps_ecoule >= temps_jeu) {
            actif = false;
            // Pour l'affichage final, on fixe le temps écoulé à la durée maximale
            temps_ecoule = temps_jeu;
        }

        // Calcul du temps restant (uniquement pour l'affichage)
        Uint32 temps_restant = temps_jeu - temps_ecoule;


        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                actif = false;
            } else if (evenement.type == SDL_KEYDOWN) {
                if (evenement.key.keysym.sym >= SDLK_a && evenement.key.keysym.sym <= SDLK_z) {
                    if (index_lettre < colonnes_grille) {
                        tentative[index_lettre++] = toupper(evenement.key.keysym.sym);
                    }
                } else if (evenement.key.keysym.sym == SDLK_BACKSPACE) {
                    if (index_lettre > 0) {
                        tentative[--index_lettre] = '\0';
                    }
                } else if (evenement.key.keysym.sym == SDLK_RETURN) {
                    if (index_lettre == colonnes_grille) {
                        propositions[ligne_actuelle] = strdup(tentative);
                        check_word(tentative, mot_secret, resultat[ligne_actuelle]);
                        ligne_actuelle++;
                        index_lettre = 0;
                        memset(tentative, 0, (colonnes_grille + 1) * sizeof(char));
                        if (ligne_actuelle >= lignes_grille || strcmp(propositions[ligne_actuelle - 1], mot_secret) == 0) {
                            actif = false;
                        }
                    }
                }
            }
        }

        // Dessin de l'image de fond
        SDL_RenderCopy(renderer, texture_fond, NULL, NULL);

        int largeur_ecran, hauteur_ecran;
        SDL_GetWindowSize(fenetre, &largeur_ecran, &hauteur_ecran);

        // Affichage du titre
        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Surface* surface_titre = TTF_RenderText_Blended(police, "MOTUS", blanc);
        SDL_Texture* texture_titre = SDL_CreateTextureFromSurface(renderer, surface_titre);
        SDL_Rect rect_titre = {largeur_ecran / 2 - 100, 20, 200, 50};
        SDL_RenderCopy(renderer, texture_titre, NULL, &rect_titre);
        SDL_FreeSurface(surface_titre);
        SDL_DestroyTexture(texture_titre);

        // Affichage du chronomètre
        afficher_chrono(renderer, police, temps_restant, largeur_ecran);

        // Dessin de la grille
        int largeur_case = 60;
        int hauteur_case = 60;
        int depart_x = (largeur_ecran - colonnes_grille * largeur_case) / 2;
        int depart_y = 150;

        draw_grid(renderer, lignes_grille, colonnes_grille, depart_x, depart_y, largeur_case, hauteur_case, resultat, propositions, tentative, ligne_actuelle);

        // Si le temps est écoulé ou la partie est terminée, afficher le mot secret
        if (!actif) {
            afficher_mot_final(renderer, police, mot_secret, largeur_ecran, hauteur_ecran);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);  // Délai de 5 secondes pour afficher le mot secret
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    // Libération de la mémoire
    SDL_DestroyTexture(texture_fond);
    Mix_HaltMusic();
    Mix_FreeMusic(musique);
    TTF_CloseFont(police);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    free(mot_secret);
    free(tentative);
    for (int i = 0; i < lignes_grille; i++) {
        free(resultat[i]);
        if (propositions[i]) {
            free((void*)propositions[i]);
        }
    }
    free(resultat);
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--sdl") == 0) {
        return start_motus_front();
    } else {
        char* secret_word = get_random_word(argv[1]);
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
                    Sleep(2000);
                }
                printf("\n");

                free(result);
            }
        }
        free(secret_word);
        return 0;
    }
}