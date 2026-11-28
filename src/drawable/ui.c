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

static SDL_AppResult _setup_move_btn(const LayoutGrid *grid, const SDL_FPoint *grid_pos,
                                     const int btn, const char *label, GameState *gs, const EMoveType *move_type)
{
    SDL_FPoint pos;

    const int x = btn % grid->num_item_x;
    const int y = btn / grid->num_item_x;

    layout_grid_item(grid, grid_pos, x, y, &pos);

    const SDL_AppResult result = btn_setup(&_buttons[btn], &_btn_ctx, label, BTN_W, BTN_H, _callback, gs, (void *)move_type);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }
    btn_pos(&_buttons[btn], &pos);

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _setup_move_btns(GameState *gs, const SDL_FRect *container)
{
    SDL_AppResult result;
    SDL_FPoint grid_pos;

    LayoutGrid grid = {
        .num_item_x = 3,
        .num_item_y = 2,
        .item_w = BTN_W,
        .item_h = BTN_H,
        .space = 2.0,
    };

    layout_grid_center(&grid, container, &grid_pos);

    result = _setup_move_btn(&grid, &grid_pos, 0, "L", gs, MOVE_SEQ_LEFT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = _setup_move_btn(&grid, &grid_pos, 1, "F", gs, MOVE_SEQ_FORWARD);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = _setup_move_btn(&grid, &grid_pos, 2, "R", gs, MOVE_SEQ_RIGHT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = _setup_move_btn(&grid, &grid_pos, 3, "HL", gs, MOVE_SEQ_HARD_LEFT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = _setup_move_btn(&grid, &grid_pos, 4, "T", gs, MOVE_SEQ_TURN);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = _setup_move_btn(&grid, &grid_pos, 5, "HR", gs, MOVE_SEQ_HARD_RIGHT);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

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