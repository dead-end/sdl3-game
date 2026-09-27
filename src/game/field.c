#include <SDL3/SDL.h>

#include "drawable.h"
#include "hexagon.h"
#include "field.h"

/**
 * The function allocates the fields
 */
SDL_AppResult field_init(GameState *gs)
{
    //
    // Allocate the memory
    //
    gs->field = SDL_calloc(gs->fields_num.x, sizeof(Field *));
    if (gs->field == NULL)
    {
        SDL_Log("Unable to allocate fields");
        return SDL_APP_FAILURE;
    }

    for (int x = 0; x < gs->fields_num.x; x++)
    {
        gs->field[x] = SDL_calloc(gs->fields_num.y, sizeof(Field));
        if (gs->field[x] == NULL)
        {
            SDL_Log("Unable to allocate fields");
            return SDL_APP_FAILURE;
        }
    }

    //
    // Initialize the fields
    //
    for (int x = 0; x < gs->fields_num.x; x++)
    {
        for (int y = 0; y < gs->fields_num.y; y++)
        {
            gs->field[x][y].hex.x = x;
            gs->field[x][y].hex.y = y;
        }
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function frees the fields.
 */
void field_cleanup(GameState *gs)
{
    if (!gs->field)
    {
        return;
    }
    for (int x = 0; x < gs->fields_num.x; x++)
    {
        SDL_free(gs->field[x]);
    }

    SDL_free(gs->field);
    gs->field = NULL;
}

/**
 * The method checks if a hex coordinate is on the board.
 */
bool field_is_valid(const GameState *gs, const SDL_Point hex)
{
    return 0 <= hex.x && hex.x < gs->fields_num.x &&
           0 <= hex.y && hex.y < gs->fields_num.y;
}