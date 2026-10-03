#include <SDL3/SDL.h>

#include "game_state.h"

/**
 * Cube coordinates of a hexagon.
 */
typedef struct Cube
{
    int q;
    int r;
    int s;
} Cube;

/**
 * Cube coordinates of a hexagon as double.
 */
typedef struct DCube
{
    double q;
    double r;
    double s;
} DCube;

/**
 * axial coordinates to offset coordinates
 */
static SDL_Point _axial_2_Off(const SDL_Point hex)
{
    return (SDL_Point){
        .x = hex.x,
        .y = hex.y + (hex.x - (hex.x % 2)) / 2,
    };
};

/**
 * cube coordinates to axial coordinates
 */
static SDL_Point _cube_2_axial(const Cube cube)
{
    return (SDL_Point){
        .x = cube.q,
        .y = cube.r,
    };
};

/**
 * axial coordinates to cube coordinates
 */
static DCube _axial_2_cube(const SDL_FPoint fAxial)
{
    return (DCube){
        .q = fAxial.x,
        .r = fAxial.y,
        .s = -fAxial.x - fAxial.y,
    };
};

/**
 * Round cube coordinates to the next
 */
static Cube _cube_round(const DCube dCube)
{
    int q = SDL_round(dCube.q);
    int r = SDL_round(dCube.r);
    int s = SDL_round(dCube.s);

    const double q_diff = SDL_fabs(q - dCube.q);
    const double r_diff = SDL_fabs(r - dCube.r);
    const double s_diff = SDL_fabs(s - dCube.s);

    if (q_diff > r_diff && q_diff > s_diff)
    {
        q = -r - s;
    }
    else if (r_diff > s_diff)
    {
        r = -q - s;
    }
    else
    {
        s = -q - r;
    }

    return (Cube){q, r, s};
}

/**
 * The function does the rounding.
 */
static SDL_Point _axial_round(SDL_FPoint fAxial)
{
    return _cube_2_axial(_cube_round(_axial_2_cube(fAxial)));
};

/**
 * The function computes for a given point on the canvas, which hex it is
 * pointing to. The origin is the center of the (0, 0) hexagon and size is
 * the size of the hexagon.
 *
 * See: https://www.redblobgames.com/grids/hexagons/
 */
SDL_Point hex_from_pixel(const GameState *gs, const SDL_FPoint mouseRel)
{
    //
    // The function is called with the mouse position relative to the camera.
    // We need the absolute position with the origin, which is the center of
    // the top left hexagon.
    //
    const SDL_FPoint mouseAbs = {
        .x = mouseRel.x + gs->camera.x - gs->field_origin.x,
        .y = mouseRel.y + gs->camera.y - gs->field_origin.y};

    //
    // ⎡q⎤     ⎡   2/3         0    ⎤   ⎛ ⎡x⎤        ⎞
    // ⎢ ⎥  =  ⎢                    ⎥ × ⎜ ⎢ ⎥ ÷ size ⎥
    // ⎣r⎦     ⎣  -1/3    sqrt(3)/3 ⎦   ⎝ ⎣y⎦        ⎠
    //
    SDL_FPoint fAxial = {
        .x = ((2.0 / 3.0) * mouseAbs.x) / gs->size,
        .y = ((-1.0 / 3.0) * mouseAbs.x + (SQRT_D_3 / 3.0) * mouseAbs.y) / gs->size,
    };

    return _axial_2_Off(_axial_round(fAxial));
}
