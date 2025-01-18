#include "../include/front/grid.h"
#include "../include/SDL/SDL.h"
#include "../include/SDL/SDL_ttf.h"
#include <string.h>

void draw_grid(SDL_Renderer* renderer, int lignes_grille, int colonnes_grille, int depart_x, int depart_y, int largeur_case, int hauteur_case, int** result, const char* propositions[], const char* tentative_en_cours, int ligne_actuelle) {
    // Activer le blending pour prendre en compte la transparence
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Color rouge = {255, 0, 0, 102};  // Rouge avec 40% de transparence
    SDL_Color jaune = {255, 255, 0, 102};  // Jaune avec 40% de transparence
    SDL_Color bleu = {36, 170, 252, 102};  // Bleu avec 40% de transparence
    SDL_Color noir = {0, 0, 0, 255};  // Noir (pas de transparence pour les bordures)
    SDL_Color blanc = {255, 255, 255, 255};  // Blanc (pas de transparence pour le texte)

    for (int ligne = 0; ligne < lignes_grille; ligne++) {
        for (int colonne = 0; colonne < colonnes_grille; colonne++) {
            SDL_Rect case_rect = {
                depart_x + colonne * largeur_case,
                depart_y + ligne * hauteur_case,
                largeur_case,
                hauteur_case
            };

            // Couleur de fond de la case
            if (ligne < ligne_actuelle) {
                switch (result[ligne][colonne]) {
                    case 2:
                        SDL_SetRenderDrawColor(renderer, rouge.r, rouge.g, rouge.b, rouge.a);
                        break;
                    case 1:
                        SDL_SetRenderDrawColor(renderer, jaune.r, jaune.g, jaune.b, jaune.a);
                        break;
                    default:
                        SDL_SetRenderDrawColor(renderer, bleu.r, bleu.g, bleu.b, bleu.a);
                        break;
                }
            } else {
                SDL_SetRenderDrawColor(renderer, bleu.r, bleu.g, bleu.b, bleu.a);
            }

            // Remplir la case avec la couleur de fond (transparente)
            SDL_RenderFillRect(renderer, &case_rect);

            // Dessiner les bordures de la case (sans transparence)
            SDL_SetRenderDrawColor(renderer, noir.r, noir.g, noir.b, noir.a);
            SDL_RenderDrawRect(renderer, &case_rect);

            // Affichage de la lettre
            char lettre[2] = {'\0', '\0'};
            if (ligne == ligne_actuelle && tentative_en_cours && colonne < strlen(tentative_en_cours)) {
                lettre[0] = tentative_en_cours[colonne];
            } else if (propositions[ligne] && propositions[ligne][colonne] != '\0') {
                lettre[0] = propositions[ligne][colonne];
            }

            if (lettre[0] != '\0') {
                TTF_Font* police = TTF_OpenFont("./assets/fonts/FranceTVBrown-Regular.ttf", 24);
                if (police) {
                    SDL_Surface* surface_texte = TTF_RenderText_Blended(police, lettre, blanc);
                    SDL_Texture* texture_texte = SDL_CreateTextureFromSurface(renderer, surface_texte);

                    SDL_Rect rect_texte = {
                        case_rect.x + largeur_case / 4,
                        case_rect.y + hauteur_case / 4,
                        largeur_case / 2,
                        hauteur_case / 2
                    };

                    SDL_RenderCopy(renderer, texture_texte, NULL, &rect_texte);
                    SDL_FreeSurface(surface_texte);
                    SDL_DestroyTexture(texture_texte);
                    TTF_CloseFont(police);
                }
            }
        }
    }
}