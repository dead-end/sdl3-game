#include <SDL3/SDL.h>

#include "drawable.h"
#include "hexagon.h"
#include "field.h"

static SDL_Texture *_texture = NULL;

/**
 * The function draws the hexagons of the fields. Most of the edges of the grid
 * are edges of two adjacent hexagons. We want to draw these edges only once.
 * To implement this a hexagon draws the edge only if the adjacent hexagon is
 * not initialized.
 *
 * The function is a little long. We need an array with the initialized state
 * of each field. It is simpler to allocate this array on the stack.
 */
static SDL_AppResult _fields_render(GameState *gs)
{
    //
    // Set the color for all lines
    //
    if (!SDL_SetRenderDrawColor(gs->renderer, 0, 128, 0, 128))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // We need an array the shows which fields are initialized.
    //
    bool initialized[gs->fields_num.x][gs->fields_num.y];
    for (int x = 0; x < gs->fields_num.x; x++)
    {
        for (int y = 0; y < gs->fields_num.y; y++)
        {
            initialized[x][y] = false;
        }
    }

    SDL_FPoint center;
    //
    // We process the hexagons of all fields.
    //
    for (int x = 0; x < gs->fields_num.x; x++)
    {
        for (int y = 0; y < gs->fields_num.y; y++)
        {
            Field *field = &gs->field[x][y];

            //
            // The center of the hexagon is the same for all 6 edges.
            //
            field_Center(gs, &field->hex, &center);

            for (int i = 0; i < 6; i++)
            {

                const SDL_Point neighbor = hex_neighbor(field->hex, i);
                const bool is_valid = field_is_valid(gs, neighbor);

                //
                // If the hexagon has no neighbor in this direction or the hexagon in
                // this direction is not initialized we draw the edge.
                //
                if (!is_valid || !initialized[neighbor.x][neighbor.y])
                {
                    const SDL_FPoint start = hex_corner(gs, &center, i);
                    const SDL_FPoint end = hex_corner(gs, &center, i + 1);

                    if (!SDL_RenderLine(gs->renderer, start.x, start.y, end.x, end.y))
                    {
                        SDL_Log("SDL_RenderLine: %s", SDL_GetError());
                        return SDL_APP_FAILURE;
                    }
                }
                initialized[field->hex.x][field->hex.y] = true;
            }
        }
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function creates the texture.
 */
static SDL_AppResult _init(GameState *gs)
{

    //
    // TODO: ??
    //
    if (!SDL_SetRenderDrawBlendMode(gs->renderer, SDL_BLENDMODE_NONE))
    {
        SDL_Log("SDL_SetRenderDrawBlendMode: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Create the texture
    //
    _texture = SDL_CreateTexture(
        gs->renderer,
        gs->pixelFormat,
        SDL_TEXTUREACCESS_TARGET,
        gs->board_w,
        gs->board_h);
    if (!_texture)
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
    // Do the rendering
    //
    const SDL_AppResult result = _fields_render(gs);
    if (SDL_APP_CONTINUE != result)
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
 * The function copies the camera part of the texture to the renderer.
 */
static SDL_AppResult _render(GameState *gs)
{
    const SDL_FRect rect = {
        .x = gs->camera.x,
        .y = gs->camera.y,
        .w = gs->camera.w,
        .h = gs->camera.h,
    };
    if (!SDL_RenderTexture(gs->renderer, _texture, &rect, NULL))
    {
        SDL_Log("SDL_RenderTexture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function frees the texture.
 */
static void _cleanup()
{
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
    }
    _texture = NULL;
}

/**
 * The function creates the Drawable.
 */
Drawable Hexagons_Create()
{
    SDL_Log("Hexagons: create");

    Drawable d = {0};
    d.init = _init;
    d.event = NULL;
    d.update = NULL;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}