#define SDL_MAIN_HANDLED
#include "./include/SDL/SDL.h"
#include "./include/SDL/SDL_ttf.h"
#include "./include/SDL/SDL_mixer.h"  

static const char* FONT_PATH         = "../assets/font.ttf";

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Erreur d'initialisation de SDL: %s", SDL_GetError());
        return 1;
    }
    // Création d'une fenêtre
    SDL_Window *ecran = SDL_CreateWindow("Motus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!ecran) {
        SDL_Log("Erreur lors de la création de la fenêtre: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création d'un renderer (optionnel mais recommandé pour SDL2)
    SDL_Renderer *renderer = SDL_CreateRenderer(
        ecran, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        SDL_Log("Erreur lors de la création du renderer: %s", SDL_GetError());
        SDL_DestroyWindow(ecran);
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

    // Boucle principale
    SDL_Event event;
    int active = 1;

    while (active) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    active = 0;
                    break;
            }
        }

        // Effacer l'écran avec une couleur
        SDL_SetRenderDrawColor(renderer, 36, 170, 252, 255);
        SDL_RenderClear(renderer);

        // Ajouter ici tout code de rendu
                SDL_Color colorWhite = {255, 255, 255, 255};
        SDL_Surface* surfaceText = TTF_RenderText_Blended(font, "Motus", colorWhite);
        SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

        // Récupère la taille du texte
        int textW = 0, textH = 0;
        SDL_QueryTexture(textureText, NULL, NULL, &textW, &textH);
        SDL_Rect dstRect = { 300, 50, textW, textH };

        // Blit du texte
        SDL_RenderCopy(renderer, textureText, NULL, &dstRect);

        // Libère surface/texture pour éviter les fuites
        SDL_FreeSurface(surfaceText);
        SDL_DestroyTexture(textureText);

        SDL_RenderPresent(renderer); // Afficher le contenu
                // Pour éviter de saturer le CPU
        SDL_Delay(16); // ~60 FPS
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ecran);
    SDL_Quit();
    return 0;
}
