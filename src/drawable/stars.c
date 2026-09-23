#include <SDL3/SDL.h>

#include <stdlib.h>

#include "drawable.h"

#define NUM_STARS 100

typedef struct
{
    float x, y;
    float size;
    Uint8 alpha;
} Star;

static Star *_stars = NULL;

/**
 * The function initializes the drawable.
 */
static SDL_AppResult _init(SDL_Renderer *renderer)
{
    int w, h;

    //
    // Allocate memory
    //
    _stars = SDL_calloc(NUM_STARS, sizeof(Star));
    if (!_stars)
    {
        SDL_Log("Unable to allocate stars!");
        return SDL_APP_FAILURE;
    }

    //
    // Store the window size.
    //
    if (!SDL_GetRenderOutputSize(renderer, &w, &h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Initialize the stars
    //
    for (int i = 0; i < NUM_STARS; i++)
    {
        _stars[i].x = (float)(rand() % w);
        _stars[i].y = (float)(rand() % h);
        _stars[i].size = (float)((rand() % 2) + 1);
        _stars[i].alpha = (Uint8)((rand() % 155) + 100);
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
 * The function does the drawing of the stars.
 */
static SDL_AppResult _render(SDL_Renderer *renderer)
{
    // TODO: Reset at the end?
    if (!SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
    {
        SDL_Log("SDL_SetRenderDrawBlendMode: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    for (int i = 0; i < NUM_STARS; i++)
    {
        if (!SDL_SetRenderDrawColor(renderer, 255, 255, 255, _stars[i].alpha))
        {
            SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_FRect starRect = {_stars[i].x, _stars[i].y, _stars[i].size, _stars[i].size};

        if (!SDL_RenderFillRect(renderer, &starRect))
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
    d.update = NULL;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}