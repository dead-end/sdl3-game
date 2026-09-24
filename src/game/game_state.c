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
    // init camera
    //
    gs->camera.x = 0;
    gs->camera.y = 0;
    if (!SDL_GetRenderOutputSize(renderer, &gs->camera.w, &gs->camera.h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // board size
    //
    gs->board_w = 2 * gs->camera.w;
    gs->board_h = 2 * gs->camera.h;

    return SDL_APP_CONTINUE;
}

/**
 * The function moves the camera position and ensures that the camera is inside
 * the board.
 */
void gs_camera_move(GameState *gs, float rel_x, float rel_y)
{
    gs->camera.x -= rel_x;
    if (gs->camera.x < 0)
    {
        gs->camera.x = 0;
    }
    else if (gs->camera.x > gs->board_w - gs->camera.w)
    {
        gs->camera.x = gs->board_w - gs->camera.w;
    }

    gs->camera.y -= rel_y;
    if (gs->camera.y < 0)
    {
        gs->camera.y = 0;
    }
    else if (gs->camera.y > gs->board_h - gs->camera.h)
    {
        gs->camera.y = gs->board_h - gs->camera.h;
    }
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
