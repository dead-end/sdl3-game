#include <SDL3/SDL.h>

#include "game_state.h"
#include "move.h"
#include "field.h"

/**
 * The function initializes the move struct for a movement in a direction.
 */
void move_direction_init(const GameState *gs, Move *move, const SDL_Point *neighbor, const SDL_FRect *src_corner, const float speed)
{
    move->type = E_MOVE_DIRECTION;
    move->speed = speed;

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
 * The function initializes the move struct for a rotation.
 */
void move_rotate_init(Move *move, const int target_direction, const float speed)
{
    move->type = E_MOVE_ROTATE;
    move->speed = speed;

    //
    // Ensure that target direction is positive
    //
    move->rotate.dst_direction = target_direction % 6;
    if (move->rotate.dst_direction < 0)
    {
        move->rotate.dst_direction += 6;
    }

    move->rotate.dst_angle = move->rotate.dst_direction * 60.0;
}