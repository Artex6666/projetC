#ifndef RENDER_H
#define RENDER_H

#include "../SDL/SDL.h"
#include "../SDL/SDL_ttf.h"

void draw_grid(SDL_Renderer* renderer, int lignes_grille, int colonnes_grille, int depart_x, int depart_y, int largeur_case, int hauteur_case, int** result, const char* propositions[], const char* tentative_en_cours, int ligne_actuelle);
void afficher_mot_final(SDL_Renderer* renderer, TTF_Font* police, const char* mot_secret, int largeur_ecran, int hauteur_ecran);

#endif