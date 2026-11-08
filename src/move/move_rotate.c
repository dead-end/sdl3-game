
#include <SDL3/SDL.h>

#include "move.h"

/**
 * The function initializes the move struct for a rotation.
 */
// TODO: not used
// TODO: user MoveRotate instead of Move
void move_rotate_init(Move *move, const int target_direction, const float speed)
{
    move->type = E_MOVE_ROTATE;
    move->rotate.speed = speed;

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

/**
 * The function computes the angle for a rotation. The SDL_RenderTextureRotated
 * function requires an angle between [0, 360).
 *
 * The function returns true if we have not reached the target angle.
 */
bool move_rotate(double *current_angle, const double target_angle, const float speed, const double delta_time)
{
    //
    // Compute the difference between the current and the target angle and
    // normalize it to [-180, 180]
    //
    float delta = target_angle - *current_angle;
    while (delta < -180.0f)
    {
        delta += 360.0f;
    }
    while (delta > 180.0f)
    {
        delta -= 360.0f;
    }

    float step = speed * delta_time;

    //
    // If our step is less than the delta we use the target
    //
    if (SDL_fabsf(delta) <= step)
    {
        *current_angle = target_angle;
        return false;
    }

    //
    // Normal step
    //
    if (delta > 0.0f)
    {
        *current_angle += step;
    }
    else
    {
        *current_angle -= step;
    }

    //
    // normalize the angle to [0, 360) for SDL
    //
    while (*current_angle < 0.0f)
    {
        *current_angle += 360.0f;
    }
    while (*current_angle >= 360.0f)
    {
        *current_angle -= 360.0f;
    }

    return true;
}