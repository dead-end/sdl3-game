#include <SDL3/SDL.h>

#include "drawable.h"
#include "button.h"
#include "layout.h"
#include "move_seq.h"
#include "spaceship.h"

static SDL_FRect _ui_frect;

#define NUM_BTN 6
static Button _buttons[NUM_BTN];

static ButtonCtx _btn_ctx;

#define BTN_W 52
#define BTN_H 52

static SDL_AppResult _callback(void *data1, void *data2)
{
    GameState *gs = (GameState *)data1;
    const EMoveType *move = (const EMoveType *)data2;
    spaceship_move(gs, move);
    return SDL_APP_CONTINUE;
}

static SDL_AppResult _setup_move_btns(GameState *gs, const SDL_FRect *container)
{
    SDL_AppResult result;

    LayoutGrid grid = {
        .num_item_x = 3,
        .num_item_y = 2,
        .item_w = BTN_W,
        .item_h = BTN_H,
        .space = 2.0,
    };

    SDL_FPoint grid_pos, pos;

    layout_grid_center(&grid, container, &grid_pos);

    //
    // left - 0, 0
    //
    layout_grid_item(&grid, &grid_pos, 0, 0, &pos);
    result = btn_setup(&_buttons[0], &_btn_ctx, "LL", BTN_W, BTN_H, _callback, gs, (void *)MOVE_SEQ_LEFT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[0], &pos);

    //
    // forward - 1, 0
    //
    layout_grid_item(&grid, &grid_pos, 1, 0, &pos);
    result = btn_setup(&_buttons[1], &_btn_ctx, "F", BTN_W, BTN_H, _callback, gs, (void *)MOVE_SEQ_FORWARD);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[1], &pos);

    //
    // right - 2, 0
    //
    layout_grid_item(&grid, &grid_pos, 2, 0, &pos);
    result = btn_setup(&_buttons[2], &_btn_ctx, "LR", BTN_W, BTN_H, _callback, gs, (void *)MOVE_SEQ_RIGHT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[2], &pos);

    //
    // hard left - 0, 1
    //
    layout_grid_item(&grid, &grid_pos, 0, 1, &pos);
    result = btn_setup(&_buttons[3], &_btn_ctx, "HL", BTN_W, BTN_H, _callback, gs, (void *)MOVE_SEQ_HARD_LEFT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[3], &pos);

    //
    // turn - 1, 1
    //
    layout_grid_item(&grid, &grid_pos, 1, 1, &pos);
    result = btn_setup(&_buttons[4], &_btn_ctx, "T", BTN_W, BTN_H, _callback, gs, (void *)MOVE_SEQ_TURN);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[4], &pos);

    //
    // hard right - 2, 1
    //
    layout_grid_item(&grid, &grid_pos, 2, 1, &pos);
    result = btn_setup(&_buttons[5], &_btn_ctx, "HR", BTN_W, BTN_H, _callback, gs, (void *)MOVE_SEQ_HARD_RIGHT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[5], &pos);

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _init(GameState *gs)
{
    SDL_AppResult result;
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

    result = btn_init_ctx(gs->renderer, BTN_W, BTN_H, &_btn_ctx);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = _setup_move_btns(gs, &_ui_frect);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
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
 * The function frees the memory.
 */
static void _cleanup()
{
    btn_cleanup_ctx(&_btn_ctx);
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
    d.cleanup = _cleanup;
    return d;
}