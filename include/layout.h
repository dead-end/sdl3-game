#ifndef LAYOUT_H
#define LAYOUT_H

#include <SDL3/SDL.h>

typedef struct LayoutGrid
{
    int num_item_x;
    int num_item_y;
    float item_w;
    float item_h;
    float space;
} LayoutGrid;

void layout_grid_center(const LayoutGrid *grid, const SDL_FRect *container, SDL_FPoint *pos);

void layout_grid_item(const LayoutGrid *grid, const SDL_FPoint *grid_pos, const int x, const int y, SDL_FPoint *pos);

void layout_text_frect(const SDL_FRect *rect, const char *text, const int char_w, const int char_h, SDL_FPoint *pos);

#endif