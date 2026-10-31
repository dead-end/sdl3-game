#include <SDL3/SDL.h>

#include "field.h"

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
bool field_neighbor(const GameState *gs, const SDL_Point *field, const int i, SDL_Point *neighbor)
{
    const SDL_Point *neighbors = field->x % 2 == 0 ? _NEIGHBOR_EVEN : _NEIGHBOR_ODD;

    int idx = i % 6;
    if (idx < 0)
    {
        idx += 6;
    }

    neighbor->x = field->x + neighbors[idx].x;
    neighbor->y = field->y + neighbors[idx].y;

    return field_is_valid(gs, neighbor);
};
