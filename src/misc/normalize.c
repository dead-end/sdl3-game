#include <SDL3/SDL.h>

/**
 * The function normalizes the vector with the size 1.
 */
// TODO: Not in use
void normalize(SDL_FPoint *p)
{
    float lengthSq = p->x * p->x + p->y * p->y;

    if (lengthSq > 0.0f)
    {
        float length = SDL_sqrtf(lengthSq);
        float invLength = 1.0f / length;

        p->x *= invLength;
        p->y *= invLength;
    }
}