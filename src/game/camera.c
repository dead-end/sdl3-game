#include <SDL3/SDL.h>

#include "camera.h"

/**
 * The function initializes the camera.
 */
void camera_init(Camera *camera, int w, int h)
{
    camera->x = 0;
    camera->y = 0;

    camera->last_x = 0;
    camera->last_y = 0;

    camera->w = w;
    camera->h = h;
}

/**
 * The function moves the camera position and ensures that the camera is inside
 * the board.
 */
void camera_move(Camera *camera, float board_w, float board_h, float rel_x, float rel_y)
{
    camera->x -= rel_x;
    if (camera->x < 0)
    {
        camera->x = 0;
    }
    else if (camera->x > board_w - camera->w)
    {
        camera->x = board_w - camera->w;
    }

    camera->y -= rel_y;
    if (camera->y < 0)
    {
        camera->y = 0;
    }
    else if (camera->y > board_h - camera->h)
    {
        camera->y = board_h - camera->h;
    }
}

/**
 * The function checks if the camera has moved.
 */
bool camera_has_moved(Camera *camera)
{
    return camera->x != camera->last_x || camera->y != camera->last_y;
}

/**
 * The function returns the delta of the camera movement.
 */
SDL_FPoint camera_delta_move(Camera *camera)
{
    SDL_FPoint delta = {
        .x = -camera->x + camera->last_x,
        .y = -camera->y + camera->last_y,
    };

    return delta;
}

/**
 * The function update the last camera position.
 */
void camera_update_last(Camera *camera)
{
    camera->last_x = camera->x;
    camera->last_y = camera->y;
}
