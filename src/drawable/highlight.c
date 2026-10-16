#include <SDL3/SDL.h>

#include "game_state.h"
#include "drawable.h"
#include "field.h"
#include "hexagon.h"

const SDL_FColor _color = {0.0f, 1.0f, 0.0f, 0.2f};

static SDL_Point _current = {.x = -1, .y = -1};

static SDL_Texture *_texture = NULL;

SDL_AppResult _draw_hexagon(GameState *gs,
                            SDL_FPoint *center)
{
    SDL_Vertex vertices[7];

    //
    // Center
    //
    vertices[0].position = *center;
    // gs_abs_to_camera(gs, &vertices[0].position);
    vertices[0].color = _color;

    //
    // 6 corners
    //
    for (int i = 0; i < 6; i++)
    {
        hex_corner(gs, center, i, &vertices[i + 1].position);
        //  gs_abs_to_camera(gs, &vertices[i + 1].position);
        vertices[i + 1].color = _color;
    }

    //
    // 6 triangles, 2 corners and the center
    // Note: Vertices must be connected in a consistent clockwise or counter-clockwise order.
    //
    const int indices[18] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1};

    //
    // Draw the triangles
    //
    if (!SDL_RenderGeometry(gs->renderer, NULL, vertices, 7, indices, 18))
    {
        SDL_Log("SDL_RenderGeometry: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _init(GameState *gs)
{
    //
    // Create the texture
    //
    _texture = SDL_CreateTexture(
        gs->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        SDL_ceilf(gs->width),
        SDL_ceilf(gs->height));
    if (_texture == NULL)
    {
        SDL_Log("SDL_CreateTexture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("w: %f %f h: %f %f", gs->width, SDL_ceilf(gs->width), gs->height, SDL_ceilf(gs->height));

    //
    // Replace the renderer with the texture
    //
    if (!SDL_SetRenderTarget(gs->renderer, _texture))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_AppResult result = _draw_hexagon(gs, &gs->field_origin);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    //
    // Reset the renderer
    //
    if (!SDL_SetRenderTarget(gs->renderer, NULL))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

static void _cleanup()
{
    if (_texture != NULL)
    {
        SDL_DestroyTexture(_texture);
    }
}

/**
 * The function is an event callback.
 */
static SDL_AppResult _event(GameState *gs, SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        SDL_FPoint m = {
            .x = event->motion.x,
            .y = event->motion.y};

        SDL_Point p = hex_from_pixel(gs, m);
        if (field_is_valid(gs, p))
        {
            _current.x = p.x;
            _current.y = p.y;
        }
        break;
    }

    return SDL_APP_CONTINUE;
}

/*
static SDL_AppResult _update(GameState *gs, double delta_time)
{
    (void)gs;
    return SDL_APP_CONTINUE;
}
*/

static SDL_AppResult _render(GameState *gs)
{
    /*
    if (_current.x >= 0 && _current.y >= 0)
    {
        SDL_FPoint center;

        field_Center(gs, &_current, &center);

        SDL_AppResult result = _draw_hexagon(gs, &center);
        if (result != SDL_APP_CONTINUE)
        {
            SDL_Log("SDL_RenderDebugText: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }
*/
    if (_current.x >= 0 && _current.y >= 0)
    {
        /*
        SDL_FPoint center;

        field_Center(gs, &_current, &center);

        gs_abs_to_camera(gs, &center);

        SDL_FRect dst = {
            .x = center.x - gs->field_origin.x,
            .y = center.y - gs->field_origin.y,
            .w = _texture->w,
            .h = _texture->h};
*/

        SDL_FPoint corner;

        field_Corner(gs, &_current, &corner);

        gs_abs_to_camera(gs, &corner);

        SDL_FRect dst = {
            .x = corner.x,
            .y = corner.y,
            .w = _texture->w,
            .h = _texture->h};

        //
        //
        //
        if (!SDL_RenderTexture(gs->renderer, _texture, NULL, &dst))
        {
            SDL_Log("SDL_RenderTexture: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    return SDL_APP_CONTINUE;
}

Drawable Highlight_Create()
{
    SDL_Log("Ship: create");

    Drawable d = {0};
    d.init = _init;
    d.event = _event;
    d.update = NULL;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}