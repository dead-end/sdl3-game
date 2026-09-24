#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL3/SDL.h>

#include "camera.h"

typedef struct GameState
{
    SDL_Renderer *renderer;

    Camera camera;

    float board_w;
    float board_h;
} GameState;

SDL_AppResult gs_init(GameState *gs, SDL_Renderer *renderer);

SDL_FPoint gs_board_to_camera(float board_x, float board_y, float camera_x, float camera_y);

SDL_FPoint gs_camera_to_board(float camera_x, float camera_y, float x, float y);

#endif