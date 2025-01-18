#include "../include/front/audio.h"
#include "../include/SDL/SDL_mixer.h"
#include <stdbool.h>

bool init_audio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        //fprintf(stderr, "Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        return false;
    }
    return true;
}

void play_music() {
    Mix_Music* musique = Mix_LoadMUS("./assets/musics/background.wav");
    if (!musique) {
        //fprintf(stderr, "Erreur chargement musique : %s\n", Mix_GetError());
        return;
    }
    if (Mix_PlayMusic(musique, -1) == -1) {
        //fprintf(stderr, "Erreur lecture musique : %s\n", Mix_GetError());
    }
}

void stop_music() {
    Mix_HaltMusic();
}