#include <SDL3/SDL.h>

#include "drawable.h"

static SDL_Texture *_texture = NULL;

/**
 * The function renders the drawable. It uses SDL_RenderGeometry. SDL3 has no
 * gradient.
 */
static SDL_AppResult _background_render(GameState *gs)
{
    SDL_FColor colorTop = {0.04f, 0.06f, 0.18f, 1.0f};
    SDL_FColor colorBottom = {0.00f, 0.00f, 0.00f, 1.0f};

    SDL_Vertex vertices[4];

    // top left
    vertices[0].position.x = 0.0f;
    vertices[0].position.y = 0.0f;
    vertices[0].color = colorTop;

    // top right
    vertices[1].position.x = gs->camera.w;
    vertices[1].position.y = 0.0f;
    vertices[1].color = colorTop;

    // bottom right
    vertices[2].position.x = gs->camera.w;
    vertices[2].position.y = gs->camera.h;
    vertices[2].color = colorBottom;

    // bottom left
    vertices[3].position.x = 0.0f;
    vertices[3].position.y = gs->camera.h;
    vertices[3].color = colorBottom;

    //
    // The rectangle consists of 2 triangles:
    // First: 0-1-2
    // Second: 2-3-0
    //
    const int indices[] = {0, 1, 2, 2, 3, 0};

    //
    // Renders triangles
    //
    if (!SDL_RenderGeometry(gs->renderer, NULL, vertices, 4, indices, 6))
    {
        SDL_Log("SDL_RenderGeometry: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function creates the texture.
 */
static SDL_AppResult _init(GameState *gs)
{
    //
    // Get the SDL_PixelFormat
    //
    SDL_Window *window = SDL_GetRenderWindow(gs->renderer);
    if (window == NULL)
    {
        SDL_Log("SDL_RenderGetWindow: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Uint32 systemFormat = SDL_GetWindowPixelFormat(window);
    if (SDL_PIXELFORMAT_UNKNOWN == systemFormat)
    {
        SDL_Log("SDL_GetWindowPixelFormat: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    _texture = SDL_CreateTexture(
        gs->renderer,
        systemFormat,
        SDL_TEXTUREACCESS_TARGET,
        gs->camera.w,
        gs->camera.h);
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
    const SDL_AppResult result = _background_render(gs);
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
Drawable Background_Create()
{
    SDL_Log("Background: create");

    Drawable d = {0};
    d.init = _init;
    d.update = NULL;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}