#include "../include/front/timer.h"
#include "../include/SDL/SDL.h"
#include "../include/SDL/SDL_ttf.h"
#include <stdio.h>

void afficher_chrono(SDL_Renderer* renderer, TTF_Font* police, Uint32 temps_restant, int largeur_ecran) {
    SDL_Color blanc = {255, 255, 255, 255};
    char message[50];
    int minutes = temps_restant / 60000;
    int secondes = (temps_restant % 60000) / 1000;
    snprintf(message, sizeof(message), "Temps: %02d:%02d", minutes, secondes);

    SDL_Surface* surface_texte = TTF_RenderText_Blended(police, message, blanc);
    SDL_Texture* texture_texte = SDL_CreateTextureFromSurface(renderer, surface_texte);

    SDL_Rect rect_texte = {
        largeur_ecran - 300,  // Position X (en haut à droite)
        20,                  // Position Y
        surface_texte->w,
        surface_texte->h
    };

    SDL_RenderCopy(renderer, texture_texte, NULL, &rect_texte);
    SDL_FreeSurface(surface_texte);
    SDL_DestroyTexture(texture_texte);
}