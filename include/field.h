#ifndef FIELD_H
#define FIELD_H

#include <SDL3/SDL.h>

#include "game_state.h"

SDL_AppResult field_init(GameState *gs);

void field_cleanup(GameState *gs);

bool field_is_valid(const GameState *gs, const SDL_Point *field);

void field_center(const GameState *gs, const SDL_Point *field, SDL_FPoint *center);

bool field_neighbor(const GameState *gs, const SDL_Point *field, const int i, SDL_Point *neighbor);

void field_corner(const GameState *gs, const SDL_FPoint *center, const int corner_i, SDL_FPoint *corner);

SDL_Point field_from_pixel(const GameState *gs, SDL_FPoint *mouseRel);

void field_top_left(const GameState *gs, const SDL_Point *field, SDL_FPoint *top_left);

bool field_abs_to_camera(const GameState *gs, const SDL_FRect *abs, SDL_FRect *rel);

void field_rect_corner(const GameState *gs, const SDL_Point *field, const int w, const int h, SDL_FPoint *corner);

#endif