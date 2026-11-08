#include <SDL3/SDL.h>

#include "move_seq.h"
#include "field.h"

static void move_seq_direction_init(const GameState *gs, MoveSeq *move_seq, const float speed)
{
    SDL_Point neighbor;
    if (!field_neighbor(gs, &move_seq->pose->field, move_seq->pose->direction, &neighbor))
    {
        SDL_Log("Stop moving because the target field is not valid!");
        move_seq->moving = false;
    }

    field_rect_corner(gs, &neighbor, move_seq->sprite->src.w, move_seq->sprite->src.h, &move_seq->direction.dst_corner);

    //
    // The direction vector works because we move to a neighbor, so the length
    // is always the same. If we move to an other place, then the speed will be
    // different.
    //
    move_seq->direction.direction.x = move_seq->sprite->abs.x - move_seq->direction.dst_corner.x;
    move_seq->direction.direction.y = move_seq->sprite->abs.y - move_seq->direction.dst_corner.y;

    move_seq->direction.neighbor.x = neighbor.x;
    move_seq->direction.neighbor.y = neighbor.y;

    move_seq->speed = speed;
}

static void _move_seq_rotate_init(MoveSeq *move_seq, const int target_direction, const float speed)
{
    move_seq->speed = speed;

    move_seq->rotate.dst_direction = (move_seq->pose->direction + target_direction) % 6;
    if (move_seq->rotate.dst_direction < 0)
    {
        move_seq->rotate.dst_direction += 6;
    }

    move_seq->rotate.dst_angle = move_seq->rotate.dst_direction * 60.0;
}

static void _move_seq_next(const GameState *gs, MoveSeq *move_seq)
{
    move_seq->idx++;

    switch (move_seq->sequence[move_seq->idx])
    {
    case E_M_NOT:
        move_seq->moving = false;
        break;

    case E_M_LEFT:
        _move_seq_rotate_init(move_seq, E_DIR_LEFT, 80.0);
        break;

    case E_M_RIGHT:
        _move_seq_rotate_init(move_seq, E_DIR_RIGHT, 80.0);
        break;

    case E_M_FORWARD:
        move_seq_direction_init(gs, move_seq, 1.0f);
        break;

    default:
        break;
    }
}

SDL_AppResult move_seq_update(const GameState *gs, MoveSeq *move_seq, double delta_time)
{
    switch (move_seq->sequence[move_seq->idx])
    {
    case E_M_LEFT:
    case E_M_RIGHT:
        if (!move_rotate(&move_seq->sprite->angle, move_seq->rotate.dst_angle, move_seq->speed, delta_time))
        {
            move_seq->pose->direction = move_seq->rotate.dst_direction;
            move_seq->sprite->angle = move_seq->rotate.dst_angle;
            _move_seq_next(gs, move_seq);
        }
        break;

    case E_M_FORWARD:

        if (!move_direction(&move_seq->direction.dst_corner, &move_seq->direction.direction, move_seq->speed, delta_time, &move_seq->sprite->abs))
        {
            move_seq->pose->field.x = move_seq->direction.neighbor.x;
            move_seq->pose->field.y = move_seq->direction.neighbor.y;
            _move_seq_next(gs, move_seq);
        }
        break;

    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

void move_seq_init(const GameState *gs, MoveSeq *move_seq, Pose *pose, Sprite *sprite, const EMove *sequence)
{
    move_seq->pose = pose;
    move_seq->sprite = sprite;

    move_seq->sequence = sequence;

    move_seq->idx = -1;
    move_seq->moving = true;

    _move_seq_next(gs, move_seq);
}
