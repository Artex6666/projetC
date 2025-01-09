#include "../include/SDL/SDL.h"
#include "../include/SDL/SDL_ttf.h"
#include "../include/SDL/SDL_mixer.h"
#include "../include/SDL/SDL_render.h"
#include "../include/back.h"
#include <stdbool.h>
#include <stdio.h>


static const char* FONT_PATH         = "./assets/fonts/FranceTVBrown-Regular.ttf";
static const char* BACKGROUND_MUSIC  = "./assets/musics/background.wav";

int start_motus_front(void)
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    // Initialisation de SDL_ttf (police d'écriture)
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialisation de SDL_mixer (Musique, effets sonores)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Motus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
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
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de la police d'écriture
    TTF_Font* font = TTF_OpenFont(FONT_PATH, 24);
    if (!font) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
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
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Lecture de la musique en boucle (-1 pour une boucle infinie)
    if (Mix_PlayMusic(music, -1) == -1) {
        fprintf(stderr, "Erreur lecture musique : %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Boucle principale
    bool active = true;
    SDL_Event event;
        printf("Secret_word");
        // Exemple de mot ayant la difficulté "normal"
        char* secret_word = get_random_word("hard");
        if (!secret_word) {
            printf("Impossible de récupérer un mot.\n");
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
        printf("after Secret_word");
        SDL_Color colorWhite = {255, 255, 255, 255};
        SDL_Surface* titleText = TTF_RenderText_Blended(font, "Motus", colorWhite);
        SDL_Texture* textureTitleText = SDL_CreateTextureFromSurface(renderer, titleText);
        SDL_Surface* wordText = TTF_RenderText_Blended(font, secret_word, colorWhite);
        SDL_Texture* textureWordText = SDL_CreateTextureFromSurface(renderer, wordText);
        SDL_FreeSurface(wordText);
        SDL_FreeSurface(titleText);
    while (active) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    active = false;
                    break;

                // ici la gestion des clics, clavier

                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 36, 170, 252, 255); // Bleu comme les carré du jeu TV Motus
        SDL_RenderClear(renderer);

        // Récupère la taille de la fenêtre
        int width_screen, height_screen;
        SDL_GetWindowSize(window, &width_screen, &height_screen);


        SDL_QueryTexture(textureTitleText, NULL, NULL, 0, 0);
        SDL_Rect dstTitle = { (width_screen / 2) - (width_screen / 16), (height_screen * 0.05), width_screen / 8, height_screen / 12};
        // debug pour connaitre la taille de la fenêtre : printf("W : %d H : %d", width_screen, height_screen);
        SDL_Rect dstWord = { (width_screen / 2) - (width_screen / 16), (height_screen * 0.25), width_screen / 8, height_screen / 12};
        // Blit du texte
        SDL_RenderCopy(renderer, textureTitleText, NULL, &dstTitle);
        SDL_RenderCopy(renderer, textureWordText, NULL, &dstWord);

        int gridRows = 6;
        int gridCols = 5;
        int cellWidth  = 40;
        int cellHeight = 40;
        int startX = (800  - gridCols * cellWidth)  / 2;
        int startY = (600 - gridRows * cellHeight) / 2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Grille couleur rouge

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

        // Fonction pour mettre à jour la fenêtre
        SDL_RenderPresent(renderer);

        // Pour éviter de saturer le CPU
        SDL_Delay(30);
    }
    // Libère texture pour éviter les fuites

        SDL_DestroyTexture(textureTitleText);
        SDL_DestroyTexture(textureWordText);

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
