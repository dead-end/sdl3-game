#include <SDL3/SDL.h>

#include "drawable.h"

static bool _is_dragging = false;

/**
 * The function moves the camera position and ensures that the camera is inside
 * the board.
 */
static void _camera_move(Camera *camera,
                         const float board_w, const float board_h,
                         const float rel_x, const float rel_y)
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
 * The function initialized the camera.
 */
static SDL_AppResult _init(GameState *gs)
{
    gs->camera.x = 0;
    gs->camera.y = 0;

    //
    // Get the size of the screen for the camera
    //
    if (!SDL_GetRenderOutputSize(gs->renderer, &gs->camera.w, &gs->camera.h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function is an event callback.
 */
static SDL_AppResult _event(GameState *gs, SDL_Event *event)
{
    switch (event->type)
    {

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            _is_dragging = true;
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            _is_dragging = false;
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        if (_is_dragging)
        {
            //
            // event.motion.xrel/yrel contains the movement of the mouse in
            // pixel since the last frame.
            //
            _camera_move(&gs->camera, gs->board_w, gs->board_h,
                         event->motion.xrel, event->motion.yrel);
        }
        break;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function creates the Drawable.
 */
Drawable Camera_Create()
{
    SDL_Log("Camera: create");

    Drawable d = {0};
    d.init = _init;
    d.event = _event;
    d.update = NULL;
    d.render = NULL;
    d.cleanup = NULL;
    return d;
}