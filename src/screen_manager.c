#include <SDL3/SDL.h>

#include "screen.h"
#include "screen_manager.h"

void sm_register_screen(ScreenManager *sm, ScreenType type, Screen screen)
{
    sm->screens[type] = screen;
}

/**
 * The function registers a request for a screen change. The change is not done
 * immediately. It will be processed at the beginning of the next frame, to
 * prevent errors.
 */
void sm_change_screen(ScreenManager *sm, ScreenType next)
{
    sm->next = next;
    sm->change = true;
}

/**
 * The function processes the change of the screen.
 */
void sm_process_change(ScreenManager *sm, SDL_Renderer *renderer)
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
SDL_AppResult sm_screen_event(ScreenManager *sm, SDL_Event *event)
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
SDL_AppResult sm_screen_update(ScreenManager *sm, double delta_time)
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
SDL_AppResult sm_screen_render(ScreenManager *sm, SDL_Renderer *renderer)
{
    Screen *current = &(sm->screens[sm->current]);
    if (current->render)
    {
        return current->render(current->state, renderer);
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function calls the cleanup function of the screen.
 */
void sm_screen_cleanup(ScreenManager *sm)
{
    Screen *current = &(sm->screens[sm->current]);
    if (current->cleanup)
    {
        current->cleanup(current->state);
    }
}
