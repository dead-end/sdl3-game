#include <SDL3/SDL.h>

#include "game_state.h"

static const SDL_FPoint _CORNERS[] = {
    {.x = -0.25, .y = -0.5},
    {.x = +0.25, .y = -0.5},
    {.x = +0.5, .y = 0},
    {.x = +0.25, .y = +0.5},
    {.x = -0.25, .y = +0.5},
    {.x = -0.5, .y = 0},
};

/**
 * The function is called with the center of a hex and computes its corners.
 */
SDL_FPoint hex_corner(const GameState *gs, const SDL_FPoint hex_center, const int corner_i)
{
    //
    // corner_i can be 6
    //
    const int idx = corner_i % 6;

    return (SDL_FPoint){
        .x = hex_center.x + _CORNERS[idx].x * gs->width,
        .y = hex_center.y + _CORNERS[idx].y * gs->height,
    };
};
