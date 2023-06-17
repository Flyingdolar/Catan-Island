#pragma once
// Include Standard Library

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Include User Library
#include "../inc/list.h"
#include "../inc/macros.h"
#include "../inc/structs.h"

// Global Variable
extern pGame game;

// Function Declaration

// init.c
int32_t init(void);
int32_t initPlayer(void);
int32_t initBlock(void);
int32_t initNode(void);
int32_t initRoad(void);
int32_t initHarbor(void);
int32_t initMap(void);

// action.c
int32_t settleAction();
int32_t diceAction();
int32_t buildAction();
int32_t robberAction();
int32_t buildRoad();
int32_t buildNode();
int32_t buyCard();
int32_t useCard();
int32_t bankTrade();

//random.c
int32_t randMap(void);
int32_t randDiceNum(void);
int32_t randPickBlock(void);