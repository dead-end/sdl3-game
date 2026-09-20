#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <SDL3/SDL.h>

#include "screen.h"

typedef struct ScreenManager
{
    SDL_Renderer *renderer;
    Screen current;
    bool change;
    Screen next;
} ScreenManager;

void ScreenManager_ChangeScreen(ScreenManager *sm, Screen screen);

void ScreenManager_ProcessPendingChange(ScreenManager *sm);

#endif