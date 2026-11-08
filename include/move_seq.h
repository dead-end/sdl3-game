#ifndef MOVE_SEQ_H
#define MOVE_SEQ_H

#include "move.h"
#include "pose.h"
#include "sprite.h"

typedef enum EMove
{
    E_M_NOT = 0,
    E_M_RIGHT,
    E_M_LEFT,
    E_M_FORWARD
} EMove;

typedef struct MoveSeq
{
    const EMove *sequence;
    int idx;
    bool moving;

    Pose *pose;
    Sprite *sprite;

    union
    {
        MoveRotate rotate;
        MoveDirection direction;
    };
} MoveSeq;

static const EMove MOVE_SEQ_LEFT[] = {E_M_FORWARD, E_M_LEFT, E_M_FORWARD, E_M_LEFT, E_M_NOT};
static const EMove MOVE_SEQ_RIGHT[] = {E_M_FORWARD, E_M_RIGHT, E_M_FORWARD, E_M_RIGHT, E_M_NOT};
static const EMove MOVE_SEQ_FORWARD[] = {E_M_FORWARD, E_M_FORWARD, E_M_FORWARD, E_M_FORWARD, E_M_NOT};

void move_seq_init(const GameState *gs, MoveSeq *move_seq, Pose *pose, Sprite *sprite, const EMove *sequence);

SDL_AppResult move_seq_update(const GameState *gs, MoveSeq *move_seq, double delta_time);

#endif