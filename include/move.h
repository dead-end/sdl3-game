#ifndef MOVE_H
#define MOVE_H

#include <SDL3/SDL.h>

bool move_direction(const SDL_FPoint *target, const SDL_FPoint *direction, const float speed, const double delta_time, SDL_FRect *abs);

bool move_rotate(double *current_angle, int target_direction, float speed, double delta_time);

#endif