#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include <emscripten.h>

#include "screen_manager.h"
#include "log.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint64_t last_time;
} AppContext;

/**
 * The function initializes the application.
 */
static SDL_AppResult _app_init(AppContext **ctx)
{

    log_init();

    if (ctx == NULL)
    {
        SDL_Log("Invalid AppContext pointer.");
        return SDL_APP_FAILURE;
    }

    *ctx = (AppContext *)SDL_calloc(1, sizeof(AppContext));
    if (*ctx == NULL)
    {
        SDL_Log("Unable to allocate AppContext");
        return SDL_APP_FAILURE;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Unable to initialize video: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 Game", 800, 600, 0, &(*ctx)->window, &(*ctx)->renderer))
    {
        SDL_Log("Unable to initialize window / renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    (*ctx)->last_time = SDL_GetTicks();

    sm_init((*ctx)->renderer);

    return SDL_APP_CONTINUE;
}

/**
 * Cleanup function.
 *
 * Do not call SDL_Log after SDL_Quit
 */
static void _app_cleanup(AppContext **ctx)
{
    if (ctx && *ctx)
    {

        sm_cleanup();

        if ((*ctx)->renderer)
        {
            SDL_DestroyRenderer((*ctx)->renderer);
            (*ctx)->renderer = NULL;
        }

        if ((*ctx)->window)
        {
            SDL_DestroyWindow((*ctx)->window);
            (*ctx)->window = NULL;
        }

        SDL_free(*ctx);
        *ctx = NULL;
    }

    log_cleanup();

    SDL_Quit();
}

/**
 * The method processes pending events. It stops at the first event that is not
 * SDL_APP_CONTINUE.
 */
static SDL_AppResult _app_event(AppContext *ctx)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return SDL_APP_SUCCESS;
        }
        else
        {
            SDL_AppResult res = sm_screen_event(&event);
            if (res != SDL_APP_CONTINUE)
            {
                return res;
            }
        }
    }
    return SDL_APP_CONTINUE;
}

/**
 * The function processes the update step.
 */
static SDL_AppResult _app_update(AppContext *ctx)
{
    uint64_t now = SDL_GetTicks();
    double delta_time = (double)(now - ctx->last_time) / 1000.0;
    ctx->last_time = now;

    return sm_screen_update(delta_time);
}

/**
 * The function processes the rendering.
 */
static SDL_AppResult _app_render(AppContext *ctx)
{
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    SDL_AppResult render_res = sm_screen_render(ctx->renderer);
    if (render_res != SDL_APP_CONTINUE)
    {
        return render_res;
    }

    SDL_RenderPresent(ctx->renderer);

    return SDL_APP_CONTINUE;
}

/**
 * Game loop which is called from the emscripten by requestAnimationFrame
 */
static void _main_loop_step(void *arg)
{
    AppContext **ctx = (AppContext **)arg;

    //
    // Process the events
    //
    if (SDL_APP_CONTINUE != _app_event(*ctx))
    {
        _app_cleanup(ctx);
        emscripten_cancel_main_loop();
        return;
    }

    //
    // Change the sceen
    //
    sm_process_change((*ctx)->renderer);

    //
    // Update the app
    //
    if (SDL_APP_CONTINUE != _app_update(*ctx))
    {
        _app_cleanup(ctx);
        emscripten_cancel_main_loop();
        return;
    }

    //
    // Render the app
    //
    if (SDL_APP_CONTINUE != _app_render(*ctx))
    {
        _app_cleanup(ctx);
        emscripten_cancel_main_loop();
        return;
    }
}

/**
 * Main function for WebAssembly
 */
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    AppContext *ctx = NULL;

    if (SDL_APP_CONTINUE != _app_init(&ctx))
    {
        _app_cleanup(&ctx);
        return 1;
    }

    //
    // Hands over the control to the browser.
    //
    emscripten_set_main_loop_arg(_main_loop_step, &ctx, 0, 1);

    //
    // This area is not reachable.
    //
    return 0;
}
