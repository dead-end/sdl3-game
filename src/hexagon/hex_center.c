#include <SDL3/SDL.h>

#include "game_state.h"

/**
 * The function computes the center of a hexagon on the board, based on the
 * coordinates of the origin on the canvas.
 */
SDL_FPoint hex_center(const GameState *gs, const SDL_Point hex)
{
    return (SDL_FPoint){
        .x = gs->field_origin.x + hex.x * gs->hSpace,
        .y = gs->field_origin.y + hex.y * gs->vSpace + ((hex.x % 2) * gs->vSpace) / 2,
    };
};
