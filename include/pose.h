#ifndef POSE_H
#define POSE_H

#include <SDL3/SDL.h>

typedef struct Pose
{
    SDL_Point field;
    int direction;
} Pose;

void pose_init(Pose *pose, const int x, const int y, const int direction);

#endif