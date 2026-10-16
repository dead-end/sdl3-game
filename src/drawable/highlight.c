#include <SDL3/SDL.h>

#include "game_state.h"
#include "drawable.h"
#include "field.h"
#include "hexagon.h"

//
// The highlighted field. The event function sets the values and the render
// function uses it.
//
static SDL_Point _current = {.x = -1, .y = -1};

static SDL_Texture *_texture = NULL;

/**
 * The function draws a highlighted hexagon on a texture.
 */
SDL_AppResult _draw_hexagon(GameState *gs,
                            SDL_FPoint *center)
{
    const SDL_FColor color = {0.0f, 1.0f, 0.0f, 0.2f};

    SDL_Vertex vertices[7];

    //
    // Center
    //
    vertices[0].position = *center;
    vertices[0].color = color;

    //
    // 6 corners
    //
    for (int i = 0; i < 6; i++)
    {
        hex_corner(gs, center, i, &vertices[i + 1].position);
        vertices[i + 1].color = color;
    }

    //
    // 6 triangles, 2 corners and the center
    // Note: Vertices must be connected in a consistent clockwise or
    // counter-clockwise order.
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

/**
 * Create an initialize the texture.
 */
static SDL_AppResult _init(GameState *gs)
{
    //
    // Create the texture
    //
    _texture = SDL_CreateTexture(
        gs->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        //
        // we make the width / hight a little bit larger to ensure that nothing
        // is cut off.
        //
        SDL_ceilf(gs->width),
        SDL_ceilf(gs->height));
    if (_texture == NULL)
    {
        SDL_Log("SDL_CreateTexture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Replace the renderer with the texture
    //
    if (!SDL_SetRenderTarget(gs->renderer, _texture))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Draw the hexagon on the texture
    //
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

/**
 * The function cleanups the texture.
 */
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

/**
 * The function renders the texture.
 */
static SDL_AppResult _render(GameState *gs)
{
    //
    // Check if we have a highlighted field.
    //
    if (_current.x >= 0 && _current.y >= 0)
    {
        //
        // Get the top left corner of the field and get its position on the
        // camera.
        //
        SDL_FPoint top_left;
        field_top_left(gs, &_current, &top_left);
        gs_abs_to_camera(gs, &top_left);

        SDL_FRect dst = {
            .x = top_left.x,
            .y = top_left.y,
            .w = _texture->w,
            .h = _texture->h};

        //
        // Ensure that the texture is visible
        //
        if (!SDL_HasRectIntersectionFloat(&dst, &gs->camera))
        {
            return SDL_APP_CONTINUE;
        }

        //
        // Render the texture at the computed position.
        //
        if (!SDL_RenderTexture(gs->renderer, _texture, NULL, &dst))
        {
            SDL_Log("SDL_RenderTexture: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function creates the Drawable.
 */
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