#pragma once
// Include Standard Library

#include <SDL2/SDL.h>
#include <stdbool.h>
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
extern char msg[10000];
extern int roundCount;

// Function Declaration

// input.c
int32_t readCMD(char *string, int32_t lower, int32_t uppers, int32_t tryTime);
int32_t readPos(char *string, int32_t type, int32_t tryTime);
int32_t readCard(char *string, int32_t tryTime);
int32_t printGameInfo(int32_t buildOption);

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

// random.c
int32_t randMap(void);
int32_t randDiceNum(void);
int32_t randPickBlock(void);

// setprop.c
int32_t placeNode(int32_t playerID, int32_t nodePos);
int32_t placeRoad(int32_t playerID, int32_t roadPos);
int32_t placeRobber(int32_t robberPos);

// check.c
int32_t checkNode(int32_t playerID, int32_t nodePos);
int32_t checkRoad(int32_t playerID, int32_t roadPos);
int32_t checkBuyCard(int32_t playerID);
int32_t checkUseCard(int32_t playerID, int32_t cardID);
int32_t checkBankTrade(int32_t playerID, int32_t giveRes, int32_t takeRes);
int32_t checkDiscard(int32_t playerID, int32_t discards, int32_t selectCard[5]);
int32_t checkRobber(int32_t playerID, int32_t blockPos);
int32_t checkRobPlayer(int32_t playerID, int32_t blockPos);
int32_t checkRobAct(int32_t robPlayer, int32_t blockPos);

//screen.c
int initScreen();
void event_loop(SDL_Window *win);
void draw(SDL_Window *win);
void drawNumber(int x, int y, int number);
void handleMouseClick_BUILD(int x, int y);
void handleMouseClick_ROBBER(int x, int y);
int getRobberBlockIndex(int x, int y);
SDL_Rect getEdgeRect(pBlock block, int blockIndex, int edgeIndex);

// debug.c
void printGraph(void);
