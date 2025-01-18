#ifndef TIMER_H
#define TIMER_H

#include "../SDL/SDL.h"
#include "../SDL/SDL_ttf.h"

void afficher_chrono(SDL_Renderer* renderer, TTF_Font* police, Uint32 temps_restant, int largeur_ecran);

#endif