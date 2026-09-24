#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera
{
    float x;
    float y;

    int w;
    int h;

    float last_x;
    float last_y;
} Camera;

void camera_init(Camera *camera, int w, int h);

void camera_move(Camera *camera, float board_w, float board_h, float rel_x, float rel_y);

bool camera_has_moved(Camera *camera);

SDL_FPoint camera_delta_move(Camera *camera);

void camera_update_last(Camera *camera);

#endif