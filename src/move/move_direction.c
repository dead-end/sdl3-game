
#include <SDL3/SDL.h>

#include "move.h"
#include "field.h"

/**
 * The function initializes the move struct for a movement in a direction.
 */
// TODO: not used
// TODO: use MoveDirection
void move_direction_init(const GameState *gs, Move *move, const SDL_Point *neighbor, const SDL_FRect *src_corner, const float speed)
{
    move->type = E_MOVE_DIRECTION;
    move->direction.speed = speed;

    field_rect_corner(gs, neighbor, src_corner->w, src_corner->h, &move->direction.dst_corner);

    //
    // The direction vector works because we move to a neighbor, so the length
    // is always the same. If we move to an other place, then the speed will be
    // different.
    //
    move->direction.direction.x = src_corner->x - move->direction.dst_corner.x;
    move->direction.direction.y = src_corner->y - move->direction.dst_corner.y;

    move->direction.neighbor.x = neighbor->x;
    move->direction.neighbor.y = neighbor->y;
}

/**
 * The function moves a rect along a direction vector. To have a consistent
 * speed, the direction vector has to be normalized.
 *
 * The function returns true if we have not reached the target.
 */
bool move_direction(const SDL_FPoint *target, const SDL_FPoint *direction, const float speed, const double delta_time, SDL_FRect *abs)
{
    //
    // Compute the delta.
    //
    const float dx = speed * delta_time * direction->x;
    const float dy = speed * delta_time * direction->y;

    //
    // If we are near the target, we return the target.
    //
    if (SDL_fabsf(target->x - abs->x) <= SDL_fabsf(dx) &&
        SDL_fabsf(target->y - abs->y) <= SDL_fabsf(dy))
    {
        abs->x = target->x;
        abs->y = target->y;
        return false;
    }

    abs->x -= dx;
    abs->y -= dy;
    return true;
}
