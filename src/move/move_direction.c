
#include <SDL3/SDL.h>

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
