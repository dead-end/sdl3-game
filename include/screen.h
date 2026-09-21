#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>

typedef struct Screen
{

    bool (*init)(void *state, SDL_Renderer *renderer);

    SDL_AppResult (*event)(void *state, SDL_Event *event);

    SDL_AppResult (*update)(void *state, double delta_time);

    SDL_AppResult (*render)(void *state, SDL_Renderer *renderer);

    void (*cleanup)(void *state);

    //
    // Screen specific data
    //
    void *state;
} Screen;

#endif