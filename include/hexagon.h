#ifndef HEXAGON_H
#define HEXAGON_H

#include <SDL3/SDL.h>

#include "game_state.h"

SDL_FPoint hex_center(const GameState *gs, const SDL_Point hex);

SDL_FPoint hex_corner(const GameState *gs, const SDL_FPoint hex_center, const int corner_i);

SDL_Point hex_neighbor(const SDL_Point hex, const int i);

SDL_Point hex_from_pixel(const GameState *gs, SDL_FPoint mouseRel);

#endif