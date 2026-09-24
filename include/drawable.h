#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SDL3/SDL.h>

#include "game_state.h"

typedef struct Drawable
{
    SDL_AppResult (*init)(GameState *gs);

    SDL_AppResult (*update)(GameState *gs, double delta_time);

    SDL_AppResult (*render)(GameState *gs);

    void (*cleanup)();
} Drawable;

Drawable Background_Create();

Drawable Stars_Create();

#endif