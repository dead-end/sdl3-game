#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <SDL3/SDL.h>

#include "screen.h"

typedef enum
{
    SCREEN_START,
    _TOTAL_
} ScreenType;

typedef struct ScreenManager
{
    SDL_Renderer *renderer;
    int current;
    bool change;
    int next;
    Screen screens[_TOTAL_];
} ScreenManager;

void sm_register_screen(ScreenManager *sm, ScreenType type, Screen screen);

void sm_change_screen(ScreenManager *sm, ScreenType type);

void sm_process_change(ScreenManager *sm);

SDL_AppResult sm_screen_event(ScreenManager *sm, SDL_Event *event);

SDL_AppResult sm_screen_update(ScreenManager *sm, double delta_time);

SDL_AppResult sm_screen_render(ScreenManager *sm, SDL_Renderer *renderer);

void sm_screen_cleanup(ScreenManager *sm);

#endif