#include <SDL2/SDL2.h>
#include <SDL2/SDL2_ttf.h>
#include <SDL2/SDL2_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include "front.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

// Chemins vers tes ressources
static const char* FONT_PATH         = "assets/font.ttf";
static const char* BACKGROUND_MUSIC  = "assets/background.wav";

int start_motus_front(void)
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialisation de la partie audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* ecran = SDL_CreateWindow("Motus", SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!ecran) {
        fprintf(stderr, "Erreur création fenêtre : %s\n", SDL_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(ecran);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de la police
    TTF_Font* font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ecran);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de la musique
    Mix_Music* music = Mix_LoadMUS(BACKGROUND_MUSIC);
    if (!music) {
        fprintf(stderr, "Erreur chargement musique : %s\n", Mix_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ecran);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Lecture de la musique en boucle (-1 pour boucle infinie)
    if (Mix_PlayMusic(music, -1) == -1) {
        fprintf(stderr, "Erreur lecture musique : %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ecran);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Boucle principale
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

        // Effacer l'écran
        SDL_SetRenderDrawColor(36, 170, 252, 255); // Bleu comme les carré du jeu TV Motus
        SDL_RenderClear(renderer);

        // Dessiner l'interface

        // 1. Exemple d'affichage d'un texte "Motus"
        SDL_Color colorWhite = {255, 255, 255, 255};
        SDL_Surface* surfaceText = TTF_RenderText_Blended(font, "Motus", colorWhite);
        SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

        // Récupère la taille du texte
        int textW = 0, textH = 0;
        SDL_QueryTexture(textureText, NULL, NULL, &textW, &textH);
        SDL_Rect dstRect = { (WINDOW_WIDTH - textW) / 2, 50, textW, textH };

        // Blit du texte
        SDL_RenderCopy(renderer, textureText, NULL, &dstRect);

        // Libère surface/texture pour éviter les fuites
        SDL_FreeSurface(surfaceText);
        SDL_DestroyTexture(textureText);

        // 2. Exemple d’affichage d’une “grille”
        // (juste des cases vides pour illustrer)
        int gridRows = 6;
        int gridCols = 5;
        int cellWidth  = 40;
        int cellHeight = 40;
        int startX = (WINDOW_WIDTH  - gridCols * cellWidth)  / 2;
        int startY = (WINDOW_HEIGHT - gridRows * cellHeight) / 2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // couleur rouge

        for (int row = 0; row < gridRows; row++) {
            for (int col = 0; col < gridCols; col++) {
                SDL_Rect cellRect = {
                    startX + col * cellWidth,
                    startY + row * cellHeight,
                    cellWidth,
                    cellHeight
                };
                SDL_RenderDrawRect(renderer, &cellRect);
            }
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);

        // Pour éviter de saturer le CPU
        SDL_Delay(16); // ~60 FPS
    }

    // -----------------------
    // Nettoyage et fermeture
    // -----------------------
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
