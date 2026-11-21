#include "layout.h"

/**
 * The function centers a gid inside a container.
 */
void layout_grid_center(const LayoutGrid *grid, const SDL_FRect *container, SDL_FPoint *pos)
{
    const int w = grid->num_item_x * grid->item_w + (grid->num_item_x - 1) * grid->space;
    const int h = grid->num_item_y * grid->item_h + (grid->num_item_y - 1) * grid->space;

    pos->x = (container->w - w) / 2 + container->x;
    pos->y = (container->h - h) / 2 + container->y;
}

/**
 * The function computes the position of a grid item.
 */
void layout_grid_item(const LayoutGrid *grid, const SDL_FPoint *grid_pos, const int x, const int y, SDL_FPoint *pos)
{
    pos->x = x * grid->item_w + x * grid->space + grid_pos->x;
    pos->y = y * grid->item_h + y * grid->space + grid_pos->y;
}

/**
 * The function centers a text on an SDL_Frect.
 */
void layout_text_frect(const SDL_FRect *rect, const char *text, const int char_w, const int char_h, SDL_FPoint *pos)
{
    pos->x = rect->x + (rect->w - SDL_strlen(text) * char_w) / 2.0f;
    pos->y = rect->y + (rect->h - char_h) / 2.0f;
}