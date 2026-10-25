#include <SDL3/SDL.h>

#include "drawable.h"
#include "hexagon.h"
#include "field.h"
#include "sprite.h"

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
    SDL_FRect cur;
    SDL_FRect end;
    SDL_FPoint dir;
    float speed;
    bool moving;
    SDL_Point field;
} Move;

static Move _move = {0};

void move_init(const GameState *gs, Move *move, const SDL_Point *start, const SDL_Point *end, const int w, const int h)
{

    SDL_Log("move init start: %d, %d end: %d, %d", start->x, start->y, end->x, end->y);

    SDL_FRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = w;
    srcRect.h = h;

    field_FRectOnField(gs, start, &srcRect, &move->cur);
    field_FRectOnField(gs, end, &srcRect, &move->end);

    move->dir.x = move->cur.x - move->end.x;
    move->dir.y = move->cur.y - move->end.y;

    move->speed = 1.0f;
    move->moving = true;

    move->field.x = end->x;
    move->field.y = end->y;
}

bool move_update(Move *move, const double delta_time)
{
    const float dx = move->speed * delta_time * move->dir.x;
    const float dy = move->speed * delta_time * move->dir.y;

    //    SDL_Log("dx: %f, dy: %f", dx, dy);
    //    SDL_Log("cur: %f, cur: %f", move->cur.x, move->cur.y);
    //    SDL_Log("end: %f, end: %f", move->end.x, move->end.y);

    if (SDL_fabsf(move->end.x - move->cur.x) <= SDL_fabsf(dx) &&
        SDL_fabsf(move->end.y - move->cur.y) <= SDL_fabsf(dy))
    {
        move->cur.x = move->end.x;
        move->cur.y = move->end.y;
        return false;
    }
    move->cur.x -= dx;
    move->cur.y -= dy;
    return true;
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

/**
 *
 */
bool rotate_update(double *current_angle, int target_direction, float speed, double delta_time)
{
    const double target_angle = target_direction * 60.0;

    //
    // Compute the difference between the current and the target angle and
    // normalize it to [-180, 180]
    //
    float delta = target_angle - *current_angle;
    while (delta < -180.0f)
    {
        delta += 360.0f;
    }
    while (delta > 180.0f)
    {
        delta -= 360.0f;
    }

    float step = speed * delta_time;

    //
    // If our step is less than the delta we use the target
    //
    if (SDL_fabsf(delta) <= step)
    {
        *current_angle = target_angle;
        return false;
    }

    //
    // Normal step
    //
    if (delta > 0.0f)
    {
        *current_angle += step;
    }
    else
    {
        *current_angle -= step;
    }

    //
    // normalize the angle to [0, 360) for SDL
    //
    while (*current_angle < 0.0f)
    {
        *current_angle += 360.0f;
    }
    while (*current_angle >= 360.0f)
    {
        *current_angle -= 360.0f;
    }

    return true;
}

// ---

typedef struct Starship
{
    SDL_Point field;
    int direction;

    Sprite sprite;

    double angle;
} Starship;

static Starship _starship;

/**
 * The function initializes the drawable.
 */
static SDL_AppResult _init(GameState *gs)
{
    _starship.field.x = 5;
    _starship.field.y = 5;

    _starship.direction = 0;
    _starship.angle = 0.0;

    const SDL_AppResult result = sprite_init(gs, &_starship.sprite, "assets/spaceship.png", 4, 0.15);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

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

            SDL_Point p = hex_from_pixel(gs, m);

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
                SDL_Point end;
                field_neighbor(_starship.field, _starship.direction, &end);
                move_init(gs, &_move, &_starship.field, &end, 40, 40);
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
        _rotate.rotating = rotate_update(&_starship.angle, _rotate.direction, _rotate.speed, delta_time);

        if (!_rotate.rotating)
        {
            _starship.direction = _rotate.direction;
            _starship.angle = _starship.direction * 60.0;
        }
    }

    if (_move.moving)
    {
        SDL_Log("move");
        _move.moving = move_update(&_move, delta_time);
        if (!_move.moving)
        {
            _starship.field.x = _move.field.x;
            _starship.field.y = _move.field.y;
        }
    }

    return SDL_APP_CONTINUE;
}

static SDL_AppResult _render(GameState *gs)
{
    SDL_FRect dstRect;

    if (_move.moving)
    {
        dstRect.x = _move.cur.x;
        dstRect.y = _move.cur.y;
        dstRect.w = _move.cur.w;
        dstRect.h = _move.cur.h;
    }
    else
    {
        field_FRectOnField(gs, &_starship.field, &_starship.sprite.src, &dstRect);
    }

    if (!field_FRectToCamera(gs, &dstRect))
    {
        return SDL_APP_CONTINUE;
    }

    if (!SDL_RenderTextureRotated(gs->renderer, _starship.sprite.sheet, &_starship.sprite.src, &dstRect, _starship.angle, NULL, SDL_FLIP_NONE))
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