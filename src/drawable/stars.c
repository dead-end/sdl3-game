#include <SDL3/SDL.h>

#include "drawable.h"
#include "camera.h"

#define MAX_SIZE 2

typedef struct
{
    float x, y;
    float size;
    float speed;
    Uint8 alpha;
} Star;

static Star *_stars = NULL;

static int _stars_num;

/**
 * The function initializes the drawable.
 */
static SDL_AppResult _init(GameState *gs)
{
    //
    // We calculate the number of stars with a density
    //
    _stars_num = (int)(gs->camera.w * gs->camera.h * 0.0003);

    //
    // Allocate memory
    //
    _stars = SDL_calloc(_stars_num, sizeof(Star));
    if (!_stars)
    {
        SDL_Log("Unable to allocate stars!");
        return SDL_APP_FAILURE;
    }

    //
    // Initialize the stars
    //
    for (int i = 0; i < _stars_num; i++)
    {

        _stars[i].x = SDL_rand(gs->camera.w);
        _stars[i].y = SDL_rand(gs->camera.h);
        _stars[i].speed = SDL_randf();
        _stars[i].size = SDL_rand(MAX_SIZE) + 1;
        _stars[i].alpha = SDL_rand(155) + 100;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function frees the memory.
 */
static void _cleanup()
{
    if (_stars)
    {
        SDL_free(_stars);
    }
}

/**
 * The function updates the star positions on the camera.
 */
static SDL_AppResult _update(GameState *gs, double delta_time)
{
    //
    // If the camera did not change, we have nothing to do.
    //
    if (!camera_has_moved(&gs->camera))
    {
        return SDL_APP_CONTINUE;
    }

    SDL_FPoint delta = camera_delta_move(&gs->camera);

    SDL_Log("moved: %f %f", delta.x, delta.y);

    SDL_FPoint new_star_pos;

    for (int i = 0; i < _stars_num; i++)
    {
        new_star_pos.x = _stars[i].x + delta.x * _stars[i].size / (MAX_SIZE + 1) * _stars[i].speed;
        new_star_pos.y = _stars[i].y + delta.y * _stars[i].size / (MAX_SIZE + 1) * _stars[i].speed;

        new_star_pos.x = SDL_fmodf(new_star_pos.x, gs->camera.w);
        if (new_star_pos.x < 0.0f)
        {
            new_star_pos.x += gs->camera.w;
        }

        new_star_pos.y = SDL_fmodf(new_star_pos.y, gs->camera.h);
        if (new_star_pos.y < 0.0f)
        {
            new_star_pos.y += gs->camera.h;
        }

        _stars[i].x = new_star_pos.x;
        _stars[i].y = new_star_pos.y;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function does the drawing of the stars.
 */
static SDL_AppResult _render(GameState *gs)
{
    // TODO: Reset at the end?
    if (!SDL_SetRenderDrawBlendMode(gs->renderer, SDL_BLENDMODE_BLEND))
    {
        SDL_Log("SDL_SetRenderDrawBlendMode: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < _stars_num; i++)
    {
        if (!SDL_SetRenderDrawColor(gs->renderer, 255, 255, 255, _stars[i].alpha))
        {
            SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_FRect starRect = {_stars[i].x, _stars[i].y, _stars[i].size, _stars[i].size};

        if (!SDL_RenderFillRect(gs->renderer, &starRect))
        {
            SDL_Log("SDL_RenderFillRect: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function creates the Drawable.
 */
Drawable Stars_Create()
{
    SDL_Log("Stars: create");

    Drawable d = {0};
    d.init = _init;
    d.update = _update;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}