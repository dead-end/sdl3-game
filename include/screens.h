#ifndef SCREENS_H
#define SCREENS_H

#include "screen.h"

typedef enum
{
    SCREEN_START,
    SCREEN_GAME,
    _TOTAL_
} ScreenType;

Screen ScreenStart_Create(void);

Screen ScreenGame_Create(void);

#endif