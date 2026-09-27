#ifndef FIELD_H
#define FIELD_H

#include <SDL3/SDL.h>

#include "game_state.h"

SDL_AppResult field_init(GameState *gs);

void field_cleanup(GameState *gs);

bool field_is_valid(const GameState *gs, const SDL_Point hex);

#endif