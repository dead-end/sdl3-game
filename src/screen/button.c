#include <SDL3/SDL.h>

#include "button.h"

//
// The definition of the character sizes. These are the sizes of:
// SDL_RenderDebugText
//
#define CHAR_W 8
#define CHAR_H 8

//
// The textures: normal and highlight
//
static SDL_Texture *_texture_nl = NULL;
static SDL_Texture *_texture_hl = NULL;

/**
 * The function creates a texture and renders a rect on it.
 */
static SDL_AppResult _create_texture(SDL_Renderer *renderer, SDL_Texture **texture, const int w, const int h, const SDL_Color color)
{
    //
    // Ensure that the pointer is valid
    //
    if (texture == NULL)
    {
        SDL_Log("SDL_Texture pointer is null");
        return SDL_APP_FAILURE;
    }

    //
    // Create the texture
    //
    *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w,
        h);
    if (*texture == NULL)
    {
        SDL_Log("SDL_CreateTexture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Replace the renderer with the texture
    //
    if (!SDL_SetRenderTarget(renderer, *texture))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Fill the rect
    //
    if (!SDL_SetRenderDrawColor(renderer, color.a, color.b, color.g, color.r))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_FRect rect = {
        .x = 0,
        .y = 0,
        .w = w,
        .h = h,
    };

    if (!SDL_RenderFillRect(renderer, &rect))
    {
        SDL_Log("SDL_RenderFillRect: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Reset the renderer
    //
    if (!SDL_SetRenderTarget(renderer, NULL))
    {
        SDL_Log("SDL_SetRenderTarget: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The init function creates the textures.
 */
SDL_AppResult btn_init(SDL_Renderer *renderer, const int w, const int h)
{
    SDL_AppResult result;
    SDL_Color color;

    //
    // Create texture foreground
    //
    color.a = 50;
    color.b = 50;
    color.g = 50;
    color.r = 255;

    result = _create_texture(renderer, &_texture_nl, w, h, color);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    //
    // Create texture foreground
    //
    color.a = 60;
    color.b = 60;
    color.g = 60;
    color.r = 255;

    result = _create_texture(renderer, &_texture_hl, w, h, color);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function initializes a button. No resources are allocated.
 */
SDL_AppResult btn_set(Button *btn, const char *text, SDL_AppResult (*callback)(), const int x, const int y, const int w, const int h)
{

    //
    // Ensure that the button size is not greater than the textures.
    //
    if (w > _texture_nl->w || h > _texture_nl->h)
    {
        SDL_Log("Invalid button sizes!");
        return SDL_APP_FAILURE;
    }

    btn->rect.x = x;
    btn->rect.y = y;
    btn->rect.w = w;
    btn->rect.h = h;

    btn->text = text;

    btn->txt_pos.x = x + (w - SDL_strlen(text) * CHAR_W) / 2;
    btn->txt_pos.y = y + (h - CHAR_H) / 2;

    btn->mouse = false;

    btn->texture_nl = _texture_nl;
    btn->texture_hl = _texture_hl;

    btn->callback = callback;

    return SDL_APP_CONTINUE;
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
            return btn->callback();
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
SDL_AppResult btn_render(SDL_Renderer *renderer, Button *btn)
{
    SDL_Texture *texture = btn->mouse ? btn->texture_hl : btn->texture_nl;

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
 * The function frees the texture.
 */
void btn_cleanup()
{
    //
    // texture normal
    //
    if (_texture_nl)
    {
        SDL_DestroyTexture(_texture_nl);
    }
    _texture_nl = NULL;

    //
    // texture highlight
    //
    if (_texture_hl)
    {
        SDL_DestroyTexture(_texture_hl);
    }
    _texture_hl = NULL;
}