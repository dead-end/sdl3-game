#include <SDL3/SDL.h>

#include "screen.h"
#include "screen_manager.h"

/**
 * The function registers a request for a screen change. The change is not done
 * immediately. It will be processed at the beginning of the next frame, to
 * prevent errors.
 */
void ScreenManager_ChangeScreen(ScreenManager *sm, Screen screen)
{
    sm->next = screen;
    sm->change = true;
}

/**
 * The function processes the change of the screen.
 */
void ScreenManager_ProcessPendingChange(ScreenManager *sm)
{
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
    if (sm->current.cleanup)
    {
        sm->current.cleanup(sm->current.state);
    }

    //
    // Change the screen
    //
    sm->current = sm->next;
    sm->change = false;

    //
    // Initialize the new screen
    //
    if (sm->current.init)
    {
        sm->current.init(sm->current.state, sm->renderer);
    }
}