#include <SDL3/SDL.h>

#include "button.h"
#include "layout.h"
#include "texture.h"

//
// The definition of the character sizes. These are the sizes of:
// SDL_RenderDebugText
//
#define CHAR_W 8
#define CHAR_H 8

const SDL_Color BTN_COLOR_FG = {
    .a = 50,
    .b = 50,
    .g = 50,
    .r = 255,
};

const SDL_Color BTN_COLOR_BG = {
    .a = 60,
    .b = 60,
    .g = 60,
    .r = 255,
};

/**
 * The init function creates the textures and stores them in the button context.
 */
// TODO: Maybe the context should be allocated here.
SDL_AppResult btn_init_ctx(SDL_Renderer *renderer, const int w, const int h, ButtonCtx *btn_ctx)
{
    SDL_AppResult result;

    //
    // Create the foreground texture
    //
    result = texture_create_fill(renderer, &btn_ctx->texture_nl, w, h, &BTN_COLOR_FG);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    //
    // Create the background texture
    //
    result = texture_create_fill(renderer, &btn_ctx->texture_hl, w, h, &BTN_COLOR_BG);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function initializes the button. It does not set the position.
 */
SDL_AppResult btn_setup(Button *btn, const ButtonCtx *btn_ctx, const char *text, const int w, const int h,
                        SDL_AppResult (*callback)(void *data), void *data)
{
    btn->ctx = btn_ctx;
    btn->data = data;
    btn->text = text;
    btn->callback = callback;
    btn->mouse = false;

    if (w > btn->ctx->texture_nl->w || h > btn->ctx->texture_nl->h)
    {
        SDL_Log("Invalid button sizes!");
        return SDL_APP_FAILURE;
    }

    btn->rect.w = w;
    btn->rect.h = h;

    return SDL_APP_CONTINUE;
}

/**
 * The function sets the position of a button. This requires to compute the
 * position of the text of the button.
 */
void btn_pos(Button *btn, const SDL_FPoint *pos)
{
    btn->rect.x = pos->x;
    btn->rect.y = pos->y;

    layout_text_frect(&btn->rect, btn->text, CHAR_W, CHAR_H, &btn->txt_pos);
}

/**
 * The function implements the event handling of a button. This means highlight
 * the button and call the callback function on click.
 */
SDL_AppResult btn_event(SDL_Event *event, Button *btn)
{
    SDL_FPoint mouse;

    switch (event->type)
    {
        //
        // Click event
        //
    case SDL_EVENT_MOUSE_BUTTON_DOWN:

        mouse.x = event->motion.x;
        mouse.y = event->motion.y;

        if (SDL_PointInRectFloat(&mouse, &btn->rect))
        {
            return btn->callback(btn->data);
        }
        break;

        //
        // Highlight
        //
    case SDL_EVENT_MOUSE_MOTION:
        mouse.x = event->motion.x;
        mouse.y = event->motion.y;

        btn->mouse = SDL_PointInRectFloat(&mouse, &btn->rect);

        break;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function renders a button.
 */
SDL_AppResult btn_render(SDL_Renderer *renderer, const Button *btn)
{
    SDL_Texture *texture = btn->mouse ? btn->ctx->texture_hl : btn->ctx->texture_nl;

    const SDL_FRect src = {
        .x = 0,
        .y = 0,
        .w = btn->rect.w,
        .h = btn->rect.h,
    };

    //
    // Render the background as a texture
    //
    if (!SDL_RenderTexture(renderer, texture, &src, &btn->rect))
    {
        SDL_Log("SDL_RenderTexture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Write the label
    //
    if (!SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_RenderDebugText(renderer, btn->txt_pos.x, btn->txt_pos.y, btn->text))
    {
        SDL_Log("SDL_RenderDebugText: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function frees the textures.
 */
void btn_cleanup_ctx(ButtonCtx *btn_ctx)
{
    SDL_Log("Button cleanup");

    if (!btn_ctx)
    {
        return;
    }

    //
    // texture normal
    //
    if (btn_ctx->texture_nl)
    {
        SDL_DestroyTexture(btn_ctx->texture_nl);
    }
    btn_ctx->texture_nl = NULL;

    //
    // texture highlight
    //
    if (btn_ctx->texture_hl)
    {
        SDL_DestroyTexture(btn_ctx->texture_hl);
    }
    btn_ctx->texture_hl = NULL;
}