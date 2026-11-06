#ifndef SPRITE_H
#define SPRITE_H

#include <SDL3/SDL.h>

#include "game_state.h"
#include "pose.h"

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

    //
    // The absolute position of the sprite
    //
    SDL_FRect abs;

    //
    // The angle for the rotation of the sprite
    //
    double angle;
} Sprite;

SDL_AppResult sprite_init(GameState *gs, Sprite *sprite, const char *file, const int num_frames, const double frame_duration);

void sprite_update(Sprite *sprite, const double delta_time);

void sprite_cleanup(Sprite *sprite);

void sprite_on_field(const GameState *gs, const Pose *pose, Sprite *sprite);

#endif