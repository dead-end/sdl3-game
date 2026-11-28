#ifndef MOVE_SEQ_H
#define MOVE_SEQ_H

#include "move.h"
#include "pose.h"
#include "sprite.h"

typedef enum EMoveType
{
    E_MOVE_STOP = 0,
    E_MOVE_RIGHT,
    E_MOVE_LEFT,
    E_MOVE_FORWARD
} EMoveType;

typedef struct MoveSeq
{
    const EMoveType *sequence;
    int seq_idx;
    bool is_moving;

    Pose *pose;
    Sprite *sprite;

    union
    {
        MoveRotate rotate;
        MoveDirection direction;
    };
} MoveSeq;

static const EMoveType *MOVE_SEQ_HARD_LEFT = (const EMoveType[]){E_MOVE_FORWARD, E_MOVE_LEFT, E_MOVE_FORWARD, E_MOVE_LEFT, E_MOVE_STOP};
static const EMoveType *MOVE_SEQ_HARD_RIGHT = (const EMoveType[]){E_MOVE_FORWARD, E_MOVE_RIGHT, E_MOVE_FORWARD, E_MOVE_RIGHT, E_MOVE_STOP};
static const EMoveType *MOVE_SEQ_LEFT = (const EMoveType[]){E_MOVE_FORWARD, E_MOVE_FORWARD, E_MOVE_LEFT, E_MOVE_FORWARD, E_MOVE_STOP};
static const EMoveType *MOVE_SEQ_RIGHT = (const EMoveType[]){E_MOVE_FORWARD, E_MOVE_FORWARD, E_MOVE_RIGHT, E_MOVE_FORWARD, E_MOVE_STOP};
static const EMoveType *MOVE_SEQ_FORWARD = (const EMoveType[]){E_MOVE_FORWARD, E_MOVE_FORWARD, E_MOVE_FORWARD, E_MOVE_FORWARD, E_MOVE_STOP};
static const EMoveType *MOVE_SEQ_TURN = (const EMoveType[]){E_MOVE_FORWARD, E_MOVE_LEFT, E_MOVE_LEFT, E_MOVE_LEFT, E_MOVE_STOP};

void move_seq_setup(const GameState *gs, const EMoveType *sequence, Pose *pose, Sprite *sprite, MoveSeq *move_seq);

SDL_AppResult move_seq_update(const GameState *gs, const double delta_time, MoveSeq *move_seq);

#endif