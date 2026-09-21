#include <SDL3/SDL.h>

#include "screen.h"
#include "screen_manager.h"

/**
 * The struct for the ScreenManager is private.
 */
typedef struct ScreenManager
{
    int current;
    // TODO: check if next is < 0
    bool change;
    int next;
    Screen screens[_TOTAL_];
} ScreenManager;

static ScreenManager *sm = NULL;

/**
 * The function to register new Screens
 */
static void _register_screen(ScreenType type, Screen screen)
{
    sm->screens[type] = screen;
}

/**
 * The function registers a request for a screen change. The change is not done
 * immediately. It will be processed at the beginning of the next frame, to
 * prevent errors.
 */
void sm_change_screen(ScreenType next)
{
    sm->next = next;
    sm->change = true;
}

/**
 * The function processes the change of the screen.
 */
void sm_process_change(SDL_Renderer *renderer)
{
    Screen *current;

    //
    // Check if a change of the screen is requested
    //
    if (!sm->change)
    {
        return;
    }

    //
    // Cleanup the old screen
    //
    current = &(sm->screens[sm->current]);
    if (current->cleanup)
    {
        current->cleanup(current->state);
    }

    //
    // Change the screen
    //
    sm->current = sm->next;
    sm->change = false;

    //
    // Initialize the new screen
    //
    current = &(sm->screens[sm->current]);
    if (current->init)
    {
        current->init(current->state, renderer);
    }
}

/**
 * The function calls the event function of the screen.
 */
SDL_AppResult sm_screen_event(SDL_Event *event)
{
    Screen *current = &(sm->screens[sm->current]);
    if (current->event)
    {
        return current->event(current->state, event);
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function calls the update function of the screen.
 */
SDL_AppResult sm_screen_update(double delta_time)
{
    Screen *current = &(sm->screens[sm->current]);
    if (current->update)
    {
        return current->update(current->state, delta_time);
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function calls the render function of the screen.
 */
SDL_AppResult sm_screen_render(SDL_Renderer *renderer)
{
    Screen *current = &(sm->screens[sm->current]);
    if (current->render)
    {
        return current->render(current->state, renderer);
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function initializes the ScreenManager.
 */
SDL_AppResult sm_init(SDL_Renderer *renderer)
{
    //
    // Allocate memory
    //
    sm = SDL_calloc(1, sizeof(ScreenManager));
    if (!sm)
    {
        SDL_Log("Unable to allocate ScreenManager");
        return SDL_APP_FAILURE;
    }

    //
    // Register screens
    //
    _register_screen(SCREEN_START, StartScreen_Create());

    //
    // Setup the initial screen
    //
    sm_change_screen(SCREEN_START);
    sm_process_change(renderer);

    return SDL_APP_CONTINUE;
}

/**
 * The function calls the cleanup function of the screen.
 */
void sm_cleanup()
{
    if (!sm)
    {
        return;
    }

    //
    // Call the cleanup function of the current screen
    //
    Screen *current = &(sm->screens[sm->current]);
    if (current->cleanup)
    {
        current->cleanup(current->state);
    }

    SDL_free(sm);
    sm = NULL;
}