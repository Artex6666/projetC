#include "../include/SDL/SDL.h"
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur d'initialisation de SDL : %s", SDL_GetError());
        return 1;
    }

    // Création d'une fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("Exemple SDL_GetWindowSize",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Log("Erreur de création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    bool active = true;
    SDL_Event event;

    while (active) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    active = false;
                    break;

                // Ajoute ici la gestion des clics clavier pour “Motus”
                // ex: saisir des lettres, valider mot, etc.

                default:
                    break;
            }
        }
    // Variables pour stocker les dimensions
    int width = 0, height = 0;

    // Obtention des dimensions de la fenêtre
    SDL_GetWindowSize(window, &width, &height);
    printf("Largeur : %d, Hauteur : %d\n", width, height);

    // Attente avant de quitter
    SDL_Delay(30);
    }

    // Nettoyage
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}