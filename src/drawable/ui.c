#include <SDL3/SDL.h>

#include "drawable.h"
#include "button.h"
#include "layout.h"
#include "utils.h"

static SDL_FRect _ui_frect;

#define NUM_BTN 6
static Button _buttons[NUM_BTN];

static ButtonCtx _btn_ctx;

#define BTN_W 52
#define BTN_H 52

static SDL_AppResult _callback(void *data)
{
    SDL_Log("click button: %d", PTR_TO_INT(data));
    return SDL_APP_CONTINUE;
}

static SDL_AppResult _init(GameState *gs)
{
    int w, h;
    //
    // Get the size of the screen
    //
    if (!SDL_GetRenderOutputSize(gs->renderer, &w, &h))
    {
        SDL_Log("SDL_GetRenderOutputSize: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    _ui_frect.x = gs->camera.w;
    _ui_frect.y = 0;
    _ui_frect.w = w - gs->camera.w;
    _ui_frect.h = h;

    SDL_AppResult result;

    result = btn_init_ctx(gs->renderer, BTN_W, BTN_H, &_btn_ctx);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    LayoutGrid grid = {
        .num_item_x = 3,
        .num_item_y = 2,
        .item_w = BTN_W,
        .item_h = BTN_H,
        .space = 2.0,
    };

    SDL_FPoint grid_pos, pos;

    layout_grid_center(&grid, &_ui_frect, &grid_pos);

    for (int i = 0; i < NUM_BTN; i++)
    {
        layout_grid_item(&grid, &grid_pos, i % 3, i < 3 ? 0 : 1, &pos);

        result = btn_init(&_buttons[i], &_btn_ctx, "Left", BTN_W, BTN_H, _callback, PTR_FROM_INT(i));
        if (result != SDL_APP_CONTINUE)
        {
            return result;
        }
        btn_pos(&_buttons[i], &pos);
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _render(GameState *gs)
{

    if (!SDL_SetRenderDrawColor(gs->renderer, 135, 175, 205, 255))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_RenderFillRect(gs->renderer, &_ui_frect))
    {
        SDL_Log("SDL_RenderFillRect: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_AppResult result;
    for (int i = 0; i < NUM_BTN; i++)
    {
        result = btn_render(gs->renderer, &_buttons[i]);
        if (result != SDL_APP_CONTINUE)
        {
            return result;
        }
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _event(GameState *gs, SDL_Event *event)
{
    (void)gs;

    SDL_AppResult result;

    //
    // Delegate the event to the buttons.
    //
    for (int i = 0; i < NUM_BTN; i++)
    {
        result = btn_event(event, &_buttons[i]);
        if (result != SDL_APP_CONTINUE)
        {
            return result;
        }
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function creates the Drawable.
 */
Drawable Ui_Create()
{
    SDL_Log("UI: create");

    Drawable d = {0};
    d.init = _init;
    d.event = _event;
    d.update = NULL;
    d.render = _render;
    d.cleanup = NULL;
    return d;
}