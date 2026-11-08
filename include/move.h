#ifndef MOVE_H
#define MOVE_H

#include <SDL3/SDL.h>

#include "game_state.h"

typedef enum EDirection
{
    E_DIR_LEFT = -1,
    E_DIR_RIGHT = 1
} EDirection;

typedef enum EMoveType
{
    E_MOVE_ROTATE,
    E_MOVE_DIRECTION,
    E_MOVE_NOT
} EMoveType;

typedef struct MoveRotate
{
    float speed;

    double dst_angle;
    int dst_direction;
} MoveRotate;

typedef struct MoveDirection
{
    float speed;

    SDL_FPoint dst_corner;
    SDL_FPoint direction;
    SDL_Point neighbor;

} MoveDirection;

typedef struct Move
{
    EMoveType type;

    union
    {
        MoveRotate rotate;
        MoveDirection direction;
    };
} Move;

bool move_direction(const SDL_FPoint *target, const SDL_FPoint *direction, const float speed, const double delta_time, SDL_FRect *abs);

bool move_rotate(double *current_angle, const double target_angle, const float speed, const double delta_time);

#endif