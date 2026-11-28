#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "game_state.h"
#include "move_seq.h"

void spaceship_move(GameState *gs, const EMoveType *move);

#endif