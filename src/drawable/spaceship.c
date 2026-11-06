#include <SDL3/SDL.h>

#include "drawable.h"
#include "field.h"
#include "sprite.h"
#include "move.h"

static Move _move = {0};

typedef struct Starship
{
    Pose pose;
    Sprite sprite;
} Starship;

static Starship _starship;

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

    case SDL_EVENT_KEY_DOWN:
        switch (event->key.key)
        {
        case SDLK_L:
            if (_move.type == E_MOVE_NOT)
            {
                move_rotate_init(&_move, _starship.pose.direction + E_DIR_LEFT, 80.0);
            }
            break;

        case SDLK_R:
            if (_move.type == E_MOVE_NOT)
            {
                move_rotate_init(&_move, _starship.pose.direction + E_DIR_RIGHT, 80.0);
            }
            break;

        case SDLK_F:
            if (_move.type == E_MOVE_NOT)
            {
                SDL_Point neighbor;
                if (field_neighbor(gs, &_starship.pose.field, _starship.pose.direction, &neighbor))
                {
                    move_direction_init(gs, &_move, &neighbor, &_starship.sprite.abs, 1.0f);
                }
            }
            break;
        }

        break;
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _update(GameState *gs, double delta_time)
{
    (void)gs;

    sprite_update(&_starship.sprite, delta_time);

    switch (_move.type)
    {
    case E_MOVE_ROTATE:
        if (!move_rotate(&_starship.sprite.angle, _move.rotate.dst_angle, _move.speed, delta_time))
        {
            _move.type = E_MOVE_NOT;

            _starship.pose.direction = _move.rotate.dst_direction;
            _starship.sprite.angle = _move.rotate.dst_angle;
        }
        break;

    case E_MOVE_DIRECTION:

        if (!move_direction(&_move.direction.dst_corner, &_move.direction.direction, _move.speed, delta_time, &_starship.sprite.abs))
        {
            _move.type = E_MOVE_NOT;

            _starship.pose.field.x = _move.direction.neighbor.x;
            _starship.pose.field.y = _move.direction.neighbor.y;
        }
        break;

    default:
        break;
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