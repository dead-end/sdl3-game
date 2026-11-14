#include <SDL3/SDL.h>

#include "move_seq.h"
#include "field.h"

/**
 * The function initializes the movement to a field. The movement is the
 * movement of the corner of the sprite.
 *
 * ATTENTION: If the target field is not a neighbor field, then the direction
 * vector has to be normalized to have a consistent speed.
 */
static void move_seq_direction_init(const GameState *gs, const Sprite *sprite, const SDL_Point *dst_field, const float speed, MoveDirection *move_direction)
{
    //
    // Get the corner of the sprite on the target field.
    //
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

/**
 * The function initializes the rotation.
 */
static void _move_seq_rotate_init(const Pose *pose, const int target_direction, const float speed, MoveRotate *move_rotate)
{
    move_rotate->dst_direction = (pose->direction + target_direction) % 6;
    if (move_rotate->dst_direction < 0)
    {
        move_rotate->dst_direction += 6;
    }

    move_rotate->dst_angle = move_rotate->dst_direction * 60.0;

    move_rotate->speed = speed;
}

/**
 * The function is called to initialize the next movement from the sequence.
 */
static SDL_AppResult _move_seq_next(const GameState *gs, MoveSeq *move_seq)
{
    move_seq->idx++;

    switch (move_seq->sequence[move_seq->idx])
    {
    case E_MOVE_NOT:
        move_seq->moving = false;
        break;

    case E_MOVE_LEFT:
        _move_seq_rotate_init(move_seq->pose, E_DIR_LEFT, 80.0, &move_seq->rotate);
        break;

    case E_MOVE_RIGHT:
        _move_seq_rotate_init(move_seq->pose, E_DIR_RIGHT, 80.0, &move_seq->rotate);
        break;

    case E_MOVE_FORWARD:

        //
        // Get the neighbor field in the direction.
        //
        SDL_Point dst_field;
        if (!field_neighbor(gs, &move_seq->pose->field, move_seq->pose->direction, &dst_field))
        {
            SDL_Log("Target field is not valid!");
            return SDL_APP_FAILURE;
        }
        move_seq_direction_init(gs, move_seq->sprite, &dst_field, 1.0f, &move_seq->direction);
        break;

    default:
        SDL_Log("_move_seq_next: unknown move type");
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function updates the sprite with the delta time depending on the type of movement.
 */
SDL_AppResult move_seq_update(const GameState *gs, MoveSeq *move_seq, double delta_time)
{
    switch (move_seq->sequence[move_seq->idx])
    {
    case E_MOVE_LEFT:
    case E_MOVE_RIGHT:

        if (!move_rotate(move_seq->rotate.dst_angle, move_seq->rotate.speed, delta_time, &move_seq->sprite->angle))
        {
            //
            // Update the direction if the rotation finished.
            //
            move_seq->pose->direction = move_seq->rotate.dst_direction;
            return _move_seq_next(gs, move_seq);
        }
        break;

    case E_MOVE_FORWARD:

        if (!move_direction(&move_seq->direction.dst_corner, &move_seq->direction.direction, move_seq->direction.speed, delta_time, &move_seq->sprite->abs))
        {
            //
            // Update the field if the movement to the direction finished.
            //
            move_seq->pose->field.x = move_seq->direction.neighbor.x;
            move_seq->pose->field.y = move_seq->direction.neighbor.y;
            return _move_seq_next(gs, move_seq);
        }
        break;

    default:
        SDL_Log("move_seq_update: unknown move type");
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/**
 * The function initializes a move sequence.
 */
void move_seq_init(const GameState *gs, MoveSeq *move_seq, Pose *pose, Sprite *sprite, const EMove *sequence)
{
    move_seq->pose = pose;
    move_seq->sprite = sprite;

    move_seq->sequence = sequence;

    move_seq->idx = -1;
    move_seq->moving = true;

    _move_seq_next(gs, move_seq);
}
