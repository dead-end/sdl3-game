#include <SDL3/SDL.h>

#include "screen_manager.h"
#include "screen.h"
#include "drawable.h"

#define NUM_DRAWABLES 3

typedef struct State
{
    Drawable drawables[NUM_DRAWABLES];

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

    _state->drawables[0] = Background_Create();
    _state->drawables[1] = Stars_Create();
    _state->drawables[2] = Hexagons_Create();

    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].init)
        {
            SDL_AppResult result = _state->drawables[i].init(renderer);
            if (result != SDL_APP_CONTINUE)
            {
                return result;
            }
        }
    }

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
    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].update)
        {
            SDL_AppResult result = _state->drawables[i].update(delta_time);
            if (result != SDL_APP_CONTINUE)
            {
                return result;
            }
        }
    }
    return SDL_APP_CONTINUE;
}

/**
 * The render function for the screen.
 */
static SDL_AppResult _render(SDL_Renderer *renderer)
{

    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].render)
        {
            SDL_AppResult result = _state->drawables[i].render(renderer);
            if (result != SDL_APP_CONTINUE)
            {
                return result;
            }
        }
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

    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].cleanup)
        {
            _state->drawables[i].cleanup();
        }
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
