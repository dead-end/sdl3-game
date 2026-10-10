#include <SDL3/SDL.h>

#include "screen_manager.h"
#include "screen.h"
#include "button.h"

//
// The size of the buttons
//
#define BTN_W 10 * 8 + 40
#define BTN_H 8 + 20

typedef struct State
{
    SDL_Renderer *renderer;

    Button btn_start;
    Button btn_quit;
} State;

static State *_state = NULL;

/**
 * The callback function for the start button.
 */
static SDL_AppResult _callback_start()
{
    sm_change_screen(SCREEN_GAME);
    return SDL_APP_CONTINUE;
}

/**
 * The callback function for the quit button.
 */
static SDL_AppResult _callback_quit()
{
    return SDL_APP_SUCCESS;
}

/**
 * The init function for the screen.
 */
static SDL_AppResult _init(SDL_Renderer *renderer)
{
    SDL_AppResult result;

    SDL_Log("StartScreen: init");

    _state = SDL_calloc(1, sizeof(State));
    if (!_state)
    {
        SDL_Log("Unable to allocate state!");
        return SDL_APP_FAILURE;
    }

    _state->renderer = renderer;

    //
    // Initialize the button functions
    //
    result = btn_init(renderer, BTN_W, BTN_H);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    //
    // Set the buttons
    //
    result = btn_set(&_state->btn_start, "Start", _callback_start, 100, 10, BTN_W, BTN_H);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = btn_set(&_state->btn_quit, "Quit", _callback_quit, 100, 100, BTN_W, BTN_H);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The event function for the screen.
 */
static SDL_AppResult _event(SDL_Event *event)
{
    SDL_Log("StartScreen: event");

    switch (event->type)
    {
    case SDL_EVENT_KEY_DOWN:

        switch (event->key.key)
        {
        case SDLK_ESCAPE:
            SDL_Log("ESC");
            return SDL_APP_SUCCESS;
            break;
        case SDLK_SPACE:
            SDL_Log("START");
            sm_change_screen(SCREEN_GAME);
            break;

        default:
            break;
        }
        break;
    }

    //
    // Delegate the event to the buttons.
    //
    SDL_AppResult result;

    result = btn_event(event, &_state->btn_start);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = btn_event(event, &_state->btn_quit);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The render function delegates to the buttons.
 */
static SDL_AppResult _render()
{
    SDL_AppResult result;

    result = btn_render(_state->renderer, &_state->btn_start);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = btn_render(_state->renderer, &_state->btn_quit);
    if (result != SDL_APP_CONTINUE)
    {
        return result;
    }

    return SDL_APP_CONTINUE;
}

/**
 * Cleanup function to free the screen state.
 */
static void _cleanup()
{
    SDL_Log("StartScreen: cleanup");

    btn_cleanup();

    if (_state)
    {
        SDL_free(_state);
        _state = NULL;
    }
}

/**
 * Factory function to create the screen structure.
 */
Screen ScreenStart_Create(void)
{

    SDL_Log("StartScreen: create");

    Screen s = {0};
    s.init = _init;
    s.event = _event;
    s.update = NULL;
    s.render = _render;
    s.cleanup = _cleanup;
    return s;
}
