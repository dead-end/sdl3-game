#ifndef SPRITE_H
#define SPRITE_H

#include <SDL3/SDL.h>

#include "game_state.h"

typedef struct Sprite
{
    int current_frame;
    int num_frames;
    double frame_duration;
    double animation_timer;

    SDL_Texture *sheet;

    //
    // The rect of the current frame
    //
    SDL_FRect src;
} Sprite;

SDL_AppResult sprite_init(GameState *gs, Sprite *sprite, const char *file, const int num_frames, const double frame_duration);

void sprite_update(Sprite *sprite, const double delta_time);

void sprite_cleanup(Sprite *sprite);

#endif