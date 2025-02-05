#ifndef MENU_H
#define MENU_H

#include "../SDL/SDL.h"
#include "../SDL/SDL_ttf.h"

int show_difficulty_menu();
const char* get_difficulty_level(int game_time);
int afficher_menu_difficulte(SDL_Renderer* renderer, TTF_Font* police);

#endif