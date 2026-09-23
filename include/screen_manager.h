#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <SDL3/SDL.h>

#include "screen.h"

void sm_change_screen(ScreenType type);

SDL_AppResult sm_process_change(SDL_Renderer *renderer);

SDL_AppResult sm_screen_event(SDL_Event *event);

SDL_AppResult sm_screen_update(double delta_time);

SDL_AppResult sm_screen_render(SDL_Renderer *renderer);

SDL_AppResult sm_init(SDL_Renderer *renderer);

void sm_cleanup();

#endif