#pragma once

#define DEBUG_MODE
#define DEV

#define PRETEND_ZOOMED_IN false

#include <stdio.h>
#include <stdbool.h> 

#include "pd_api.h"
#include "constants.h"

enum kGameMode {kWander, kMenuSelect, kMenuOptionSelected};

extern PlaydateAPI* pd;

void setPDPtr(PlaydateAPI* _p);

int gameLoop(void* _data);

int getFrameCount(void);

enum kGameMode getGameMode(void);

void setGameMode(enum kGameMode _mode);

void initGame(void);



