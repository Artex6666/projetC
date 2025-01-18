#include "../include/front/menu.h"
#include "../include/SDL/SDL.h"
#include "../include/SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdbool.h>

int afficher_menu_difficulte(SDL_Renderer* renderer, TTF_Font* police) {
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color rouge = {255, 0, 0, 255};

    const char* options[] = {"1. Facile (5 minutes)", "2. Moyen (2 minutes)", "3. Difficile (1 minute)"};
    int nombre_options = 3;
    int selection = 0;  // Index de l'option sélectionnée

    bool actif = true;
    SDL_Event evenement;

    while (actif) {
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_QUIT) {
                return -1;  // Quitter le jeu
            } else if (evenement.type == SDL_KEYDOWN) {
                switch (evenement.key.keysym.sym) {
                    case SDLK_UP:
                        // Déplacer la sélection vers le haut
                        selection--;
                        if (selection < 0) {
                            selection = nombre_options - 1;  // Revenir à la dernière option
                        }
                        break;
                    case SDLK_DOWN:
                        // Déplacer la sélection vers le bas
                        selection++;
                        if (selection >= nombre_options) {
                            selection = 0;  // Revenir à la première option
                        }
                        break;
                    case SDLK_RETURN:
                        // Valider la sélection
                        switch (selection) {
                            case 0:
                                return 5 * 60 * 1000;  // 5 minutes
                            case 1:
                                return 2 * 60 * 1000;  // 2 minutes
                            case 2:
                                return 1 * 60 * 1000;  // 1 minute
                        }
                        break;
                    case SDLK_ESCAPE:
                        return -1;  // Quitter le menu
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 36, 170, 252, 255);
        SDL_RenderClear(renderer);

        int largeur_ecran, hauteur_ecran;
        SDL_GetWindowSize(SDL_GetWindowFromID(1), &largeur_ecran, &hauteur_ecran);

        // Affichage des options du menu
        for (int i = 0; i < nombre_options; i++) {
            SDL_Surface* surface_texte = TTF_RenderText_Blended(police, options[i], (i == selection) ? rouge : blanc);
            SDL_Texture* texture_texte = SDL_CreateTextureFromSurface(renderer, surface_texte);

            SDL_Rect rect_texte = {
                (largeur_ecran - surface_texte->w) / 2,
                200 + i * 50,  // Position Y de chaque option
                surface_texte->w,
                surface_texte->h
            };

            SDL_RenderCopy(renderer, texture_texte, NULL, &rect_texte);
            SDL_FreeSurface(surface_texte);
            SDL_DestroyTexture(texture_texte);
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    return -1;  // Ne devrait jamais être atteint
}