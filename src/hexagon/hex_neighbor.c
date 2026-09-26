#include <SDL3/SDL.h>

/**
 * We are using a odd-q hex model as described in:
 * https://www.redblobgames.com/grids/hexagons/
 */
static const SDL_Point _NEIGHBOR_EVEN[] = {
    {.x = +0, .y = -1},
    {.x = +1, .y = -1},
    {.x = +1, .y = +0},
    {.x = +0, .y = +1},
    {.x = -1, .y = +0},
    {.x = -1, .y = -1},
};

static const SDL_Point _NEIGHBOR_ODD[] = {
    {.x = +0, .y = -1},
    {.x = +1, .y = +0},
    {.x = +1, .y = +1},
    {.x = +0, .y = +1},
    {.x = -1, .y = +1},
    {.x = -1, .y = -0},
};

/**
 * The function returns the coordinates of neighbor of a hexagon in a given
 * direction.
 */
SDL_Point hex_neighbor(const SDL_Point hex, const int i)
{
    const SDL_Point *neighbors = hex.x % 2 == 0 ? _NEIGHBOR_EVEN : _NEIGHBOR_ODD;

    const int idx = i % 6;

    return (SDL_Point){
        .x = hex.x + neighbors[idx].x,
        .y = hex.y + neighbors[idx].y,
    };
};
