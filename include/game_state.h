#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL3/SDL.h>

typedef struct GameState
{
    SDL_Renderer *renderer;

    SDL_FRect camera;

    // TODO: maybe FSize
    float board_w;
    float board_h;
} GameState;

SDL_AppResult gs_init(GameState *gs, SDL_Renderer *renderer);

void gs_camera_move(GameState *gs, float rel_x, float rel_y);

SDL_FPoint gs_board_to_camera(float board_x, float board_y, float camera_x, float camera_y);

SDL_FPoint gs_camera_to_board(float camera_x, float camera_y, float x, float y);

#endif