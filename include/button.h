#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>

typedef struct Button
{
    SDL_FRect rect;

    const char *text;
    SDL_FPoint txt_pos;

    bool mouse;

    SDL_Texture *texture_nl;
    SDL_Texture *texture_hl;

    SDL_AppResult (*callback)();

} Button;

SDL_AppResult btn_init(SDL_Renderer *renderer, const int w, const int h);

SDL_AppResult btn_set(Button *btn, const char *text, SDL_AppResult (*callback)(), const int x, const int y, const int w, const int h);

SDL_AppResult btn_render(SDL_Renderer *renderer, Button *btn);

SDL_AppResult btn_event(SDL_Event *event, Button *btn);

void btn_cleanup();

#endif