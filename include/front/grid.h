#ifndef GRID_H
#define GRID_H

#include "../SDL/SDL.h"

void draw_grid(SDL_Renderer* renderer, int lignes_grille, int colonnes_grille, int depart_x, int depart_y, int largeur_case, int hauteur_case, int** result, const char* propositions[], const char* tentative_en_cours, int ligne_actuelle);

#endif