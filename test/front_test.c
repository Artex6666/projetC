#define SDL_MAIN_HANDLED
#include "include/SDL/SDL.h"
#include "include/SDL/SDL_ttf.h"
#include "include/SDL/SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>

static const char* FONT_PATH = "assets/fonts/FranceTVBrown-Regular.ttf";

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Erreur d'initialisation de SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        SDL_Log("Erreur d'initialisation de SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        SDL_Log("Erreur d'initialisation de SDL_mixer: %s", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window *ecran = SDL_CreateWindow("Motus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!ecran) {
        SDL_Log("Erreur lors de la création de la fenêtre: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(ecran, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Erreur lors de la création du renderer: %s", SDL_GetError());
        SDL_DestroyWindow(ecran);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

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

    SDL_Color colorWhite = {255, 255, 255, 255};
    SDL_Surface* surfaceText = TTF_RenderText_Blended(font, "Motus", colorWhite);
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);

    SDL_Event event;
    int active = 1;

    while (active) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                active = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 36, 170, 252, 255);
        SDL_RenderClear(renderer);

        int textW = 0, textH = 0;
        SDL_QueryTexture(textureText, NULL, NULL, &textW, &textH);
        SDL_Rect dstRect = { 300, 50, textW, textH };
        SDL_RenderCopy(renderer, textureText, NULL, &dstRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Limiter à 60 FPS
    }

    SDL_DestroyTexture(textureText);
    TTF_CloseFont(font);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ecran);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
