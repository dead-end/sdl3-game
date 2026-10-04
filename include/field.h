#ifndef FIELD_H
#define FIELD_H

#include <SDL3/SDL.h>

#include "game_state.h"

SDL_AppResult field_init(GameState *gs);

void field_cleanup(GameState *gs);

bool field_is_valid(const GameState *gs, const SDL_Point hex);

void field_Center(const GameState *gs, const SDL_Point *field, SDL_FPoint *center);

bool field_FRectToCamera(const GameState *gs, SDL_FRect *absRect);

void field_FRectOnField(const GameState *gs, const SDL_Point *field, const SDL_FRect *src, SDL_FRect *dst);

#endif