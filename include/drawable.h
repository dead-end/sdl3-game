#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SDL3/SDL.h>

typedef struct Drawable
{
    SDL_AppResult (*init)(SDL_Renderer *renderer);

    SDL_AppResult (*update)(double delta_time);

    SDL_AppResult (*render)(SDL_Renderer *renderer);

    void (*cleanup)();
} Drawable;

Drawable Background_Create();

Drawable Stars_Create();

#endif