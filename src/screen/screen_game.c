#include <SDL3/SDL.h>

#include "screen_manager.h"
#include "screen.h"
#include "drawable.h"
#include "game_state.h"
#include "field.h"

#define NUM_DRAWABLES 5

// TODO: maybe we can store the parameter separately.
typedef struct State
{
    Drawable drawables[NUM_DRAWABLES];
    GameState gs;

} State;

static State *_state = NULL;

/**
 * The init function for the screen.
 */
static SDL_AppResult _init(SDL_Renderer *renderer)
{
    (void)renderer;

    SDL_AppResult result;

    SDL_Log("GameScreen: init");

    _state = SDL_calloc(1, sizeof(State));
    if (!_state)
    {
        SDL_Log("Unable to allocate state!");
        return SDL_APP_FAILURE;
    }

    result = gs_init(&_state->gs, renderer);
    if (SDL_APP_CONTINUE != result)
    {
        return result;
    }

    //
    // The fields are allocated separately
    //
    result = field_init(&_state->gs);
    if (SDL_APP_CONTINUE != result)
    {
        return result;
    }

    _state->drawables[0] = Camera_Create();
    _state->drawables[1] = Background_Create();
    _state->drawables[2] = Stars_Create();
    _state->drawables[3] = Hexagons_Create();
    _state->drawables[4] = Ship_Create();

    //
    // Delegate the init call
    //
    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].init)
        {
            result = _state->drawables[i].init(&_state->gs);
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

    //
    // Check for quit or screen change
    //
    switch (event->type)
    {
    case SDL_EVENT_KEY_DOWN:
        switch (event->key.key)
        {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
            break;

        case SDLK_SPACE:
            sm_change_screen(SCREEN_START);
            break;
        }
        break;
    }

    //
    // Delegate the event
    //
    SDL_AppResult result;

    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].event)
        {
            result = _state->drawables[i].event(&_state->gs, event);
            if (result != SDL_APP_CONTINUE)
            {
                return result;
            }
        }
    }

    return SDL_APP_CONTINUE;
}

/**
 * The update function for the screen.
 */
static SDL_AppResult _update(double delta_time)
{
    SDL_AppResult result;

    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].update)
        {
            result = _state->drawables[i].update(&_state->gs, delta_time);
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
static SDL_AppResult _render()
{
    SDL_AppResult result;

    for (int i = 0; i < NUM_DRAWABLES; i++)
    {
        if (_state->drawables[i].render)
        {
            result = _state->drawables[i].render(&_state->gs);
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

    //
    // The fields are allocated separately
    //
    field_cleanup(&_state->gs);

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
