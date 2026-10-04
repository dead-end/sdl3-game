#include <SDL3/SDL.h>

#include "drawable.h"
#include "hexagon.h"
#include "field.h"

typedef struct Starship
{
    SDL_Point hex;
    int animation_idx;

    int current_frame;
    int num_frames;
    double frame_duration;
    double animation_timer;
} Starship;

static Starship _starship;

static SDL_Texture *spriteSheet = NULL;

/**
 * The function initializes the drawable.
 */
static SDL_AppResult _init(GameState *gs)
{
    SDL_Surface *surface = SDL_LoadPNG("assets/spaceship.png");
    if (!surface)
    {
        SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    spriteSheet = SDL_CreateTextureFromSurface(gs->renderer, surface);
    if (!spriteSheet)
    {
        SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);

    _starship.hex.x = 1;
    _starship.hex.y = 1;

    _starship.current_frame = 0;
    _starship.num_frames = 4;
    _starship.frame_duration = 0.15;
    _starship.animation_timer = 0.0;

    return SDL_APP_CONTINUE;
}

static void _cleanup()
{
    if (spriteSheet)
    {
        SDL_DestroyTexture(spriteSheet);
        spriteSheet = NULL;
    }
}

/**
 * The function is an event callback.
 */
static SDL_AppResult _event(GameState *gs, SDL_Event *event)
{
    switch (event->type)
    {

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            SDL_FPoint m = {
                .x = event->motion.x,
                .y = event->motion.y};

            SDL_Point p = hex_from_pixel(gs, m);

            SDL_Log("field: %d / %d mouse: %f / %f", p.x, p.y, m.x, m.y);
        }
        break;
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _update(GameState *gs, double delta_time)
{
    (void)gs;

    _starship.animation_timer += delta_time;

    if (_starship.animation_timer >= _starship.frame_duration)
    {
        _starship.animation_timer -= _starship.frame_duration;
        _starship.current_frame = (_starship.current_frame + 1) % _starship.num_frames;
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _render(GameState *gs)
{
    SDL_FRect srcRect;
    srcRect.x = 40 * _starship.current_frame;
    srcRect.y = 0;
    srcRect.w = 40;
    srcRect.h = 40;

    SDL_FRect dstRect;

    field_FRectOnField(gs, &_starship.hex, &srcRect, &dstRect);

    if (!field_FRectToCamera(gs, &dstRect))
    {
        return SDL_APP_CONTINUE;
    }

    if (!SDL_RenderTexture(gs->renderer, spriteSheet, &srcRect, &dstRect))
    {
        SDL_Log("SDL_RenderCopy: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

Drawable Spaceship_Create()
{
    SDL_Log("Ship: create");

    Drawable d = {0};
    d.init = _init;
    d.event = _event;
    d.update = _update;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}