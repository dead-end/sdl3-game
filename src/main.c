//
// Activates SDL3 callback system
//
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include "screen_manager.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    ScreenManager screen_manager;
    uint64_t last_time;
} AppContext;

/**
 * By default emsdl uses stderr to write logs. This is mapped to
 * console.error(). Using printf writes to console.log().
 */
void LogOutputFunction(void *userdata __attribute__((unused)), int category __attribute__((unused)), SDL_LogPriority priority __attribute__((unused)),
                       const char *message)
{
    printf("%s\n", message);
}

/**
 * SDL callback function to initialize the app.
 */
SDL_AppResult SDL_AppInit(void **appstate,
                          int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    //
    // Register our LogOutputFunction to log to console.log()
    //
    SDL_SetLogOutputFunction(LogOutputFunction, NULL);

    //
    // Allocate AppState
    //
    AppContext *ctx = SDL_calloc(1, sizeof(AppContext));
    if (!ctx)
    {
        SDL_Log("Unable to allocate memory for AppState: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    *appstate = ctx;

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
    if (!SDL_CreateWindowAndRenderer("SDL3 WASM Game", 800, 600, 0, &ctx->window, &ctx->renderer))
    {
        SDL_Log("Unable to create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    ctx->last_time = SDL_GetTicks();

    sm_register_screen(&ctx->screen_manager, SCREEN_START, StartScreen_Create());
    sm_change_screen(&ctx->screen_manager, SCREEN_START);
    sm_process_change(&ctx->screen_manager, ctx->renderer);

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

    AppContext *ctx = (AppContext *)appstate;

    return sm_screen_event(&ctx->screen_manager, event);
}

/**
 * SDL callback function which is call in the game-loop / rendering (once a frame)
 */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_AppResult res;
    AppContext *ctx = (AppContext *)appstate;
    ScreenManager *sm = &ctx->screen_manager;

    //
    // Process pending screen changes
    //
    sm_process_change(sm, ctx->renderer);

    //
    // Compute delta time
    //
    uint64_t now = SDL_GetTicks();
    double delta_time = (double)(now - ctx->last_time) / 1000.0;
    ctx->last_time = now;

    //
    // Update with delta time
    //
    res = sm_screen_update(sm, delta_time);
    if (res != SDL_APP_CONTINUE)
    {
        return res;
    }

    //
    // Rendering
    //
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    res = sm_screen_render(sm, ctx->renderer);
    if (res != SDL_APP_CONTINUE)
    {
        return res;
    }

    SDL_RenderPresent(ctx->renderer);
    return SDL_APP_CONTINUE;
}

/**
 * SDL callback function to cleanup before quit.
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

    if (result == SDL_APP_FAILURE)
    {
        SDL_Log("App finished due to a failure!");
    }
    if (appstate)
    {
        AppContext *ctx = (AppContext *)appstate;

        sm_screen_cleanup(&ctx->screen_manager);

        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        SDL_free(ctx);
    }
    SDL_Quit();
}
