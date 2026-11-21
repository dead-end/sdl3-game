#include <SDL3/SDL.h>

/**
 * The function creates a texture which is a simple rect with one color.
 */
SDL_AppResult texture_create_fill(SDL_Renderer *renderer, SDL_Texture **texture, const int w, const int h, const SDL_Color color)
{
    //
    // Ensure that the pointer is valid
    //
    if (texture == NULL)
    {
        SDL_Log("SDL_Texture pointer is null");
        return SDL_APP_FAILURE;
    }

    //
    // Create the texture
    //
    *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w,
        h);
    if (*texture == NULL)
    {
        SDL_Log("SDL_CreateTexture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Replace the renderer with the texture
    //
    if (!SDL_SetRenderTarget(renderer, *texture))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Fill the rect
    //
    if (!SDL_SetRenderDrawColor(renderer, color.a, color.b, color.g, color.r))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_FRect rect = {
        .x = 0,
        .y = 0,
        .w = w,
        .h = h,
    };

    if (!SDL_RenderFillRect(renderer, &rect))
    {
        SDL_Log("SDL_RenderFillRect: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Reset the renderer
    //
    if (!SDL_SetRenderTarget(renderer, NULL))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}
