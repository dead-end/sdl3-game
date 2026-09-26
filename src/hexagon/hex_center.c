#include <SDL3/SDL.h>

#include "game_state.h"

/**
 * The function computes the center of a hexagon on the board, based on the
 * coordinates of the origin on the canvas.
 */
SDL_FPoint hex_center(const GameState *gs, const SDL_Point hex)
{

    const float origin_x = gs->width / 2 - gs->camera.x;
    const float origin_y = gs->height / 2 - gs->camera.y;

    return (SDL_FPoint){
        .x = origin_x + hex.x * gs->hSpace,
        .y = origin_y + hex.y * gs->vSpace + ((hex.x % 2) * gs->vSpace) / 2,
    };
};
