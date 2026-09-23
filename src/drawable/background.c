#include <SDL3/SDL.h>

#include "drawable.h"

/**
 * The function renders the drawable. It uses SDL_RenderGeometry. SDL3 has no
 * gradient.
 */
static SDL_AppResult _render(SDL_Renderer *renderer)
{
    int w, h;
    if (!SDL_GetRenderOutputSize(renderer, &w, &h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float width = (float)w;
    float height = (float)h;

    SDL_FColor colorTop = {0.04f, 0.06f, 0.18f, 1.0f};
    SDL_FColor colorBottom = {0.00f, 0.00f, 0.00f, 1.0f};

    SDL_Vertex vertices[4];

    // top left
    vertices[0].position.x = 0.0f;
    vertices[0].position.y = 0.0f;
    vertices[0].color = colorTop;

    // top right
    vertices[1].position.x = width;
    vertices[1].position.y = 0.0f;
    vertices[1].color = colorTop;

    // bottom right
    vertices[2].position.x = width;
    vertices[2].position.y = height;
    vertices[2].color = colorBottom;

    // bottom left
    vertices[3].position.x = 0.0f;
    vertices[3].position.y = height;
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
    if (!SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6))
    {
        SDL_Log("SDL_RenderGeometry: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function creates the Drawable.
 */
Drawable Background_Create()
{
    SDL_Log("Background: create");

    Drawable d = {0};
    d.init = NULL;
    d.update = NULL;
    d.render = _render;
    d.cleanup = NULL;
    return d;
}