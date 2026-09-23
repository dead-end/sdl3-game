#ifndef SCREEN_H
#define SCREEN_H

#include <SDL3/SDL.h>

typedef struct Screen
{

    SDL_AppResult (*init)(SDL_Renderer *renderer);

    SDL_AppResult (*event)(SDL_Event *event);

    SDL_AppResult (*update)(double delta_time);

    SDL_AppResult (*render)(SDL_Renderer *renderer);

    void (*cleanup)();
} Screen;

typedef enum
{
    SCREEN_START,
    SCREEN_GAME,
    _TOTAL_
} ScreenType;

Screen ScreenStart_Create(void);

Screen ScreenGame_Create(void);

#endif