#include <SDL3/SDL.h>

#include "drawable.h"
#include "field.h"
#include "sprite.h"
#include "move.h"

typedef enum EDirection
{
    E_DIR_LEFT = -1,
    E_DIR_RIGHT = 1
} EDirection;

typedef struct Rotate
{
    int direction;
    float speed;
    bool rotating;
} Rotate;

static Rotate _rotate = {0};

typedef struct Move
{
    SDL_FPoint target;
    SDL_FPoint direction;
    float speed;
    bool moving;
    SDL_Point neighbor;
} Move;

static Move _move = {0};

void move_init(const GameState *gs, Move *move, const SDL_Point *start, const SDL_Point *neighbor, const SDL_FRect *abs)
{
    SDL_Log("move init start: %d, %d neighbor: %d, %d", start->x, start->y, neighbor->x, neighbor->y);

    field_rect_corner(gs, neighbor, abs->w, abs->h, &move->target);

    //
    // The direction vector works because we move to a neighbor, so the length
    // is always the same. If we move to an other place, then the speed will be
    // different.
    //
    move->direction.x = abs->x - move->target.x;
    move->direction.y = abs->y - move->target.y;

    move->speed = 1.0f;
    move->moving = true;

    move->neighbor.x = neighbor->x;
    move->neighbor.y = neighbor->y;
}

void rotate_init(Rotate *rotate, const int target_direction, const float speed)
{
    rotate->speed = speed;

    rotate->direction = target_direction % 6;
    if (rotate->direction < 0)
    {
        rotate->direction += 6;
    }

    rotate->rotating = true;
}

typedef struct Starship
{
    SDL_Point field;
    int direction;

    Sprite sprite;

    double angle;
    SDL_FRect abs;
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

    _starship.field.x = 5;
    _starship.field.y = 5;

    _starship.direction = 0;
    _starship.angle = 0.0;

    _starship.abs.w = _starship.sprite.src.w;
    _starship.abs.h = _starship.sprite.src.h;
    field_place_frect(gs, &_starship.field, &_starship.abs);

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
            if (!_rotate.rotating)
            {
                rotate_init(&_rotate, _starship.direction + E_DIR_LEFT, 60.0);
            }
            break;

        case SDLK_R:
            if (!_rotate.rotating)
            {
                rotate_init(&_rotate, _starship.direction + E_DIR_RIGHT, 60.0);
            }
            break;

        case SDLK_F:
            if (!_move.moving)
            {
                SDL_Point neighbor;
                if (field_neighbor(gs, &_starship.field, _starship.direction, &neighbor))
                {
                    move_init(gs, &_move, &_starship.field, &neighbor, &_starship.abs);
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

    if (_rotate.rotating)
    {
        SDL_Log("rote");
        _rotate.rotating = move_rotate(&_starship.angle, _rotate.direction, _rotate.speed, delta_time);

        if (!_rotate.rotating)
        {
            _starship.direction = _rotate.direction;
            _starship.angle = _starship.direction * 60.0;
        }
    }

    if (_move.moving)
    {
        SDL_Log("move");
        _move.moving = move_direction(&_move.target, &_move.direction, _move.speed, delta_time, &_starship.abs);
        if (!_move.moving)
        {
            _starship.field.x = _move.neighbor.x;
            _starship.field.y = _move.neighbor.y;
        }
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _render(GameState *gs)
{
    SDL_FRect camera;
    if (!field_abs_to_camera(gs, &_starship.abs, &camera))
    {
        return SDL_APP_CONTINUE;
    }

    if (!SDL_RenderTextureRotated(gs->renderer, _starship.sprite.sheet, &_starship.sprite.src, &camera, _starship.angle, NULL, SDL_FLIP_NONE))
    {
        SDL_Log("SDL_RenderTextureRotated: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
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