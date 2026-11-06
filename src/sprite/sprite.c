#include <SDL3/SDL.h>

#include "game_state.h"
#include "field.h"
#include "sprite.h"

/**
 * The function initializes the sprite.
 */
SDL_AppResult sprite_init(GameState *gs, Sprite *sprite, const char *file, const int num_frames, const double frame_duration)
{
    //
    // Create the texture
    //
    SDL_Surface *surface = SDL_LoadPNG(file);
    if (!surface)
    {
        SDL_Log("SDL_LoadPNG: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    sprite->sheet = SDL_CreateTextureFromSurface(gs->renderer, surface);
    if (!sprite->sheet)
    {
        SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);

    //
    // The animation configuration
    //
    sprite->current_frame = 0;
    sprite->num_frames = num_frames;
    sprite->frame_duration = frame_duration;
    sprite->animation_timer = 0.0;

    //
    // Initialize the src rect with the first frame.
    //
    sprite->src.x = 0.0f;
    sprite->src.y = 0.0f;
    sprite->src.w = sprite->sheet->w / sprite->num_frames;
    sprite->src.h = sprite->sheet->h;

    //
    // Set the width and height of the sprite
    //
    sprite->abs.w = sprite->src.w;
    sprite->abs.h = sprite->src.h;

    return SDL_APP_CONTINUE;
}

/**
 * The function frees the texture of the sprite
 */
void sprite_cleanup(Sprite *sprite)
{
    if (sprite && sprite->sheet)
    {
        SDL_DestroyTexture(sprite->sheet);
        sprite->sheet = NULL;
    }
}

/**
 * The function changes the current frame of the sprite, if necessary.
 */
void sprite_update(Sprite *sprite, const double delta_time)
{

    sprite->animation_timer += delta_time;

    if (sprite->animation_timer >= sprite->frame_duration)
    {
        sprite->animation_timer -= sprite->frame_duration;
        sprite->current_frame = (sprite->current_frame + 1) % sprite->num_frames;

        //
        // Update the src rect if the frame changed.
        //
        sprite->src.x = sprite->sheet->w / sprite->num_frames * sprite->current_frame;
    }
}

/**
 * The function renders the sprite if it is visible.
 */
SDL_AppResult sprite_render(const GameState *gs, const Sprite *sprite)
{
    SDL_FRect camera;
    //
    // The function returns false if the intersection of the sprite and the
    // camera is empty.
    //
    if (!field_abs_to_camera(gs, &sprite->abs, &camera))
    {
        return SDL_APP_CONTINUE;
    }

    if (!SDL_RenderTextureRotated(gs->renderer, sprite->sheet, &sprite->src, &camera, sprite->angle, NULL, SDL_FLIP_NONE))
    {
        SDL_Log("SDL_RenderTextureRotated: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function places a sprite on a field.
 */
void sprite_on_field(const GameState *gs, const Pose *pose, Sprite *sprite)
{
    //
    // Get the center of the field
    //
    SDL_FPoint center;
    field_center(gs, &pose->field, &center);

    //
    // Place the rect over the center.
    //
    sprite->abs.x = center.x - (sprite->abs.w * 0.5);
    sprite->abs.y = center.y - (sprite->abs.h * 0.5);

    sprite->angle = pose->direction * 60.0;
}