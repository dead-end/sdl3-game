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
    // The origin of the field on the screen, which is the center of the top
    // left hexagon.
    //
    gs->field_origin.x = gs->width / 2.0f;
    gs->field_origin.y = gs->height / 2.0f;

    //
    // Compute board sizes
    //
    const float x = 2 * gs->size + (gs->fields_num.x - 1) * gs->hSpace;
    const float y = gs->fields_num.y * gs->vSpace + gs->vSpace / 2.0f;

    //
    // board size
    //
    gs->board_w = (int)SDL_ceilf(x);
    gs->board_h = (int)SDL_ceilf(y);

    return SDL_APP_CONTINUE;
}

/**
 * The function compute the position on the camera for the absolute position.
 * The result may not on the camera.
 */
void gs_abs_to_camera(GameState *gs, SDL_FPoint *p)
{
    p->x -= gs->camera.x;
    p->y -= gs->camera.y;
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
