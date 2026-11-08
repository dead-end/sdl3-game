#include <SDL3/SDL.h>

#include "move_seq.h"
#include "field.h"

// TODO: move to move.c
static void move_seq_direction_init(const GameState *gs, const Sprite *sprite, const SDL_Point *dst_field, const float speed, MoveDirection *move_direction)
{
    field_rect_corner(gs, dst_field, sprite->src.w, sprite->src.h, &move_direction->dst_corner);

    //
    // The direction vector works because we move to a neighbor, so the length
    // is always the same. If we move to an other place, then the speed will be
    // different.
    //
    move_direction->direction.x = sprite->abs.x - move_direction->dst_corner.x;
    move_direction->direction.y = sprite->abs.y - move_direction->dst_corner.y;

    move_direction->neighbor.x = dst_field->x;
    move_direction->neighbor.y = dst_field->y;

    move_direction->speed = speed;
}

// TODO: move to move.c
static void _move_seq_rotate_init(MoveRotate *move_rotate, Pose *pose, const int target_direction, const float speed)
{
    move_rotate->dst_direction = (pose->direction + target_direction) % 6;
    if (move_rotate->dst_direction < 0)
    {
        move_rotate->dst_direction += 6;
    }

    move_rotate->dst_angle = move_rotate->dst_direction * 60.0;

    move_rotate->speed = speed;
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
        _move_seq_rotate_init(&move_seq->rotate, move_seq->pose, E_DIR_LEFT, 80.0);
        break;

    case E_M_RIGHT:
        _move_seq_rotate_init(&move_seq->rotate, move_seq->pose, E_DIR_RIGHT, 80.0);
        break;

    case E_M_FORWARD:

        SDL_Point dst_field;
        if (!field_neighbor(gs, &move_seq->pose->field, move_seq->pose->direction, &dst_field))
        {
            SDL_Log("Stop moving because the target field is not valid!");
            move_seq->moving = false;
        }
        move_seq_direction_init(gs, move_seq->sprite, &dst_field, 1.0f, &move_seq->direction);
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
        if (!move_rotate(&move_seq->sprite->angle, move_seq->rotate.dst_angle, move_seq->rotate.speed, delta_time))
        {
            move_seq->pose->direction = move_seq->rotate.dst_direction;
            move_seq->sprite->angle = move_seq->rotate.dst_angle;
            _move_seq_next(gs, move_seq);
        }
        break;

    case E_M_FORWARD:

        if (!move_direction(&move_seq->direction.dst_corner, &move_seq->direction.direction, move_seq->direction.speed, delta_time, &move_seq->sprite->abs))
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
