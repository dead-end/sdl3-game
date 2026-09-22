#include <SDL3/SDL.h>

#include "screen_manager.h"
#include "screen.h"

typedef struct State
{
    int r;
    int direction;

} State;

static State *_state = NULL;

/**
 * The init function for the screen.
 */
static SDL_AppResult _init(SDL_Renderer *renderer)
{
    (void)renderer;

    SDL_Log("GameScreen: init");

    _state = SDL_calloc(1, sizeof(State));
    if (!_state)
    {
        SDL_Log("Unable to allocate state!");
        return SDL_APP_FAILURE;
    }

    _state->r = 100;
    _state->direction = 1;
    return SDL_APP_CONTINUE;
}

/**
 * The event function for the screen.
 */
static SDL_AppResult _event(SDL_Event *event)
{
    SDL_Log("GameScreen: event");

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.key)
        {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
            break;
        case SDLK_SPACE:
            sm_change_screen(SCREEN_START);
            break;

        default:
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

/**
 * The update function for the screen.
 */
static SDL_AppResult _update(double delta_time)
{
    //    SDL_Log("update");

    _state->r += _state->direction * delta_time * 100;

    if (_state->r > 255)
    {
        _state->r = 255;
        _state->direction = -1;
    }

    if (_state->r < 0)
    {
        _state->r = 0;
        _state->direction = 1;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The render function for the screen.
 */
static SDL_AppResult _render(SDL_Renderer *renderer)
{

    if (!SDL_SetRenderDrawColor(renderer, 0, _state->r, 0, 255))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_FRect my_rect = {
        .x = 0.0f,
        .y = 0.0f,
        .w = 100.0f,
        .h = 100.0f};

    if (!SDL_RenderFillRect(renderer, &my_rect))
    {
        SDL_Log("SDL_RenderFillRect: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255))
    {
        SDL_Log("SDL_SetRenderDrawColor: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_RenderDebugText(renderer, 10.0, 10.0, "Press space to continue ..."))
    {
        SDL_Log("SDL_RenderDebugText: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * Cleanup function to free the screen state.
 */
static void _cleanup()
{
    SDL_Log("GameScreen: cleanup");

    if (!_state)
    {
        return;
    }

    SDL_free(_state);
    _state = NULL;
}

/**
 * Factory function to create the screen structure.
 */
Screen ScreenGame_Create(void)
{
    SDL_Log("GameScreen: create");

    Screen s = {0};
    s.init = _init;
    s.event = _event;
    s.update = _update;
    s.render = _render;
    s.cleanup = _cleanup;
    return s;
}
