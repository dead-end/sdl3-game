#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL3/SDL.h>

#include "camera.h"

/**
 * The definition of the field.
 *
 * The Field is part of the GameState and the Field functions use the
 * GameState. So it is not possible to define the Field struct with the Field
 * functions.
 */
typedef struct Field
{
    SDL_Point hex;
} Field;

/**
 * The state for the game screen
 */
typedef struct GameState
{
    SDL_Renderer *renderer;
    Uint32 pixelFormat;

    Camera camera;

    float board_w;
    float board_h;

    //
    // The outer radius of the hex.
    //
    int size;

    //
    // The distance between the centers of two vertical hexagons, which means two
    // hexagon on top of each other.
    //
    float vSpace;

    //
    // The distance between the centers of two horizontal hexagons. The hexagons
    // have an offset to the top or the bottom.
    //
    float hSpace;

    //
    // The size from the left corner to the right corner.
    //
    float width;

    //
    // The size from the top to the bottom edge of the hex. This is also the
    // inner radius.
    //
    float height;

    //
    // The number of fields / hexagons.
    //
    SDL_Point fields_num;

    //
    //
    //
    Field **field;

    SDL_FPoint field_origin;
} GameState;

#define SQRT_D_3 1.7320508075688772935
#define SQRT_f_3 1.7320508f

SDL_AppResult gs_init(GameState *gs, SDL_Renderer *renderer);

SDL_FPoint gs_board_to_camera(float board_x, float board_y, float camera_x, float camera_y);

SDL_FPoint gs_camera_to_board(float camera_x, float camera_y, float x, float y);

#endif