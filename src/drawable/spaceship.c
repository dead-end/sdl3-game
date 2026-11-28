#include <SDL3/SDL.h>

#include "drawable.h"
#include "field.h"
#include "sprite.h"
#include "move_seq.h"

static MoveSeq _move_seq = {0};

typedef struct Starship
{
    Pose pose;
    Sprite sprite;
} Starship;

static Starship _starship;

void spaceship_move(GameState *gs, const EMoveType *move)
{
    if (!_move_seq.is_moving)
    {
        move_seq_init(gs, move, &_starship.pose, &_starship.sprite, &_move_seq);
    }
}

/**
 * The function initializes the drawable.
 */
static SDL_AppResult _init(GameState *gs)
{

    const SDL_AppResult result = sprite_init(gs, &_starship.sprite, "assets/spaceship.png", 4, 0.15);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    pose_init(&_starship.pose, 5, 5, 0);

    sprite_on_field(gs, &_starship.pose, &_starship.sprite);

    return SDL_APP_CONTINUE;
}

static void _cleanup()
{
    sprite_cleanup(&_starship.sprite);
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
            SDL_FPoint m = {
                .x = event->motion.x,
                .y = event->motion.y};

            SDL_Point p = field_from_pixel(gs, &m);

            SDL_Log("field: %d / %d mouse: %f / %f", p.x, p.y, m.x, m.y);
        }
        break;
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _update(GameState *gs, double delta_time)
{
    sprite_update(&_starship.sprite, delta_time);

    if (_move_seq.is_moving)
    {
        return move_seq_update(gs, delta_time, &_move_seq);
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _render(GameState *gs)
{
    return sprite_render(gs, &_starship.sprite);
}

Drawable Spaceship_Create()
{
    SDL_Log("Ship: create");

    Drawable d = {0};
    d.init = _init;
    d.event = _event;
    d.update = _update;
    d.render = _render;
    d.cleanup = _cleanup;
    return d;
}