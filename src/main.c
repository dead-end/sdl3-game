#define SDL_MAIN_USE_CALLBACKS 1 // Aktiviert das neue SDL3-Callback-System
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} AppState;

/**
 * By default emsdl uses stderr to write logs. This is mapped to
 * console.error(). Using printf writes to console.log().
 */
void LogOutputFunction(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    printf("%s\n", message);
}

/**
 * SDL callback function to initialize the app.
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    //
    // Register our LogOutputFunction to log to console.log()
    //
    SDL_SetLogOutputFunction(LogOutputFunction, NULL);

    //
    // Allocate AppState
    //
    AppState *state = SDL_calloc(1, sizeof(AppState));
    if (!state)
    {
        SDL_Log("Unable to allocate memory for AppState: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    *appstate = state;

    //
    // Initialize SDL video
    //
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //
    // Create window and renderer
    //
    if (!SDL_CreateWindowAndRenderer("SDL3 WASM Game", 800, 600, 0, &state->window, &state->renderer))
    {
        SDL_Log("Unable to create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * SDL callback function to process events
 */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/**
 * SDL callback function which is call in the game-loop / rendering (once a frame)
 */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = (AppState *)appstate;

    SDL_SetRenderDrawColor(state->renderer, 120, 40, 80, 255);
    SDL_RenderClear(state->renderer);

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

/**
 * SDL callback function to cleanup before quit.
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (appstate)
    {
        AppState *state = (AppState *)appstate;
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
    SDL_Quit();
}
