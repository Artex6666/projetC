#ifndef AUDIO_H
#define AUDIO_H

#include "../SDL/SDL_mixer.h"
#include <stdbool.h>

bool init_audio();
void play_music();
void stop_music();

#endif