#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>

SDL_AppResult texture_create_fill(SDL_Renderer *renderer, SDL_Texture **texture, const int w, const int h, const SDL_Color *color);

#endif