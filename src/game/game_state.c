#include <SDL3/SDL.h>

#include "game_state.h"

/**
 * The method initializes the GameState.
 */
SDL_AppResult gs_init(GameState *gs, SDL_Renderer *renderer)
{
    //
    // renderer
    //
    gs->renderer = renderer;

    //
    // Get the SDL_PixelFormat
    //
    SDL_Window *window = SDL_GetRenderWindow(gs->renderer);
    if (window == NULL)
    {
        SDL_Log("SDL_RenderGetWindow: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    gs->pixelFormat = SDL_GetWindowPixelFormat(window);
    if (SDL_PIXELFORMAT_UNKNOWN == gs->pixelFormat)
    {
        SDL_Log("SDL_GetWindowPixelFormat: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Get the size of the screen for the camera
    //
    int w, h;
    if (!SDL_GetRenderOutputSize(renderer, &w, &h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    camera_init(&gs->camera, w, h);

    //
    // Compute the hexagon sizes
    //
    gs->size = 40;

    gs->vSpace = SQRT_f_3 * (float)gs->size;

    gs->hSpace = 1.5 * (float)gs->size;

    gs->width = 2 * (float)gs->size;

    gs->height = SQRT_f_3 * (float)gs->size;

    //
    // The number of fields
    //
    gs->fields_num.x = 20;
    gs->fields_num.y = 10;

    //
    // Compute board sizes
    //
    const float x = 2 * gs->size + (gs->fields_num.x - 1) * gs->hSpace;
    const float y = gs->fields_num.y * gs->vSpace + gs->vSpace / 2;

    //
    // board size
    //
    gs->board_w = (int)SDL_ceilf(x);
    gs->board_h = (int)SDL_ceilf(y);

    return SDL_APP_CONTINUE;
}

/**
 * not used
 */
SDL_FPoint gs_board_to_camera(float board_x, float board_y, float camera_x, float camera_y)
{
    return (SDL_FPoint){
        .x = board_x - camera_x,
        .y = board_y - camera_y};
}

/**
 * not used
 */
SDL_FPoint gs_camera_to_board(float camera_x, float camera_y, float x, float y)
{
    return (SDL_FPoint){
        .x = camera_x + x,
        .y = camera_y + y};
}
