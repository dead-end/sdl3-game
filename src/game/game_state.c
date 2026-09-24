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

    int w, h;
    if (!SDL_GetRenderOutputSize(renderer, &w, &h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    camera_init(&gs->camera, w, h);

    //
    // board size
    //
    gs->board_w = 2 * w;
    gs->board_h = 2 * h;

    return SDL_APP_CONTINUE;
}

/**
 * not used
 */
SDL_FPoint gs_board_to_camera(float board_x, float board_y, float camera_x, float camera_y)
{
    SDL_FPoint result = {
        .x = board_x - camera_x,
        .y = board_y - camera_y};
    return result;
}

/**
 * not used
 */
SDL_FPoint gs_camera_to_board(float camera_x, float camera_y, float x, float y)
{
    SDL_FPoint result = {
        .x = camera_x + x,
        .y = camera_y + y};
    return result;
}
