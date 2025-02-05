#include "../include/front/render.h"
#include "../include/SDL/SDL.h"
#include "../include/SDL/SDL_ttf.h"
#include <stdio.h>

void afficher_mot_final(SDL_Renderer* renderer, TTF_Font* police, const char* mot_secret, int largeur_ecran, int hauteur_ecran) {
    SDL_Color blanc = {255, 255, 255, 255};
    char message[50];
    snprintf(message, sizeof(message), "Mot à trouver : %s", mot_secret);

    TTF_Font* police_finale = TTF_OpenFont("./assets/fonts/Roboto_Condensed-Regular.ttf", 32);
    if (!police_finale) {
        fprintf(stderr, "Erreur chargement police finale : %s\n", TTF_GetError());
        return;
    }

    SDL_Surface* surface_message = TTF_RenderText_Blended(police_finale, message, blanc);
    SDL_Texture* texture_message = SDL_CreateTextureFromSurface(renderer, surface_message);

    SDL_Rect rect_message = {
        (largeur_ecran - surface_message->w) / 2,
        hauteur_ecran - 100,
        surface_message->w,
        surface_message->h
    };

    SDL_RenderCopy(renderer, texture_message, NULL, &rect_message);

    SDL_FreeSurface(surface_message);
    SDL_DestroyTexture(texture_message);
    TTF_CloseFont(police_finale);
}