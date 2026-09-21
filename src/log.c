#include <SDL3/SDL.h>
#include <stdio.h>

//
// Variables to restore the initial log function
//
static SDL_LogOutputFunction _original_log_callback = NULL;

static void *_original_log_userdata = NULL;

/**
 * By default emsdl uses stderr to write logs. This is mapped to
 * console.error(). Using printf writes to console.log().
 */
void custom_log_callback(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    (void)userdata;

    printf("category %d | priority %d: %s\n", category, priority, message);
}

/**
 * The function saves the original logging function and replaces it with the
 * custom on.
 */
void log_init(void)
{
    SDL_GetLogOutputFunction(&_original_log_callback, &_original_log_userdata);

    SDL_SetLogOutputFunction(custom_log_callback, NULL);
}

/**
 * The function restores the original logging function.
 */
void log_cleanup(void)
{
    if (_original_log_callback != NULL)
    {
        SDL_SetLogOutputFunction(_original_log_callback, _original_log_userdata);
    }
}
