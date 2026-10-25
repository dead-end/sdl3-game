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

/**
 * The function computes the camera coordinates from the absolute coordinates.
 */
bool field_FRectToCamera(const GameState *gs, SDL_FRect *rect)
{
    //
    // If the rect is not on the camera rect, there is nothing to do.
    //
    if (!SDL_HasRectIntersectionFloat(rect, &gs->camera))
    {
        return false;
    }
    rect->x -= gs->camera.x;
    rect->y -= gs->camera.y;

    return true;
}

/**
 * The function computes the center of a hexagon on the board, based on the
 * coordinates of the origin on the canvas.
 */
void field_center(const GameState *gs, const SDL_Point *field, SDL_FPoint *center)
{
    center->x = gs->field_origin.x + field->x * gs->hSpace;
    center->y = gs->field_origin.y + field->y * gs->vSpace + ((field->x % 2) * gs->vSpace) / 2.0f;
};

/**
 * The function compute the position of the top left corner (0,0) of the origin
 * field to the top left corner on the current field.
 */
void field_top_left(const GameState *gs, const SDL_Point *field, SDL_FPoint *top_left)
{
    top_left->x = field->x * gs->hSpace;
    top_left->y = field->y * gs->vSpace + ((field->x % 2) * gs->vSpace) / 2.0f;
};

/**
 *
 */
void field_FRectOnField(const GameState *gs, const SDL_Point *field, const SDL_FRect *src, SDL_FRect *dst)
{
    //
    // Get the center of the field
    //
    SDL_FPoint center;
    field_center(gs, field, &center);

    //
    // Place the rect over the center.
    //
    dst->x = center.x - (src->w * 0.5);
    dst->y = center.y - (src->h * 0.5);
    dst->w = src->w;
    dst->h = src->h;
}