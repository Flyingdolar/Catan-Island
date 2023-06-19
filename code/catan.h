#pragma once
// Include Standard Library

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
int32_t readDiscard(uint8_t *hold, int32_t array[6], int32_t tryTime);
int32_t readBankTrade(int32_t *giveRes, int32_t *takeRes, int32_t tryTime);
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
int32_t randPickRoad(void);
int32_t randPickNode(void);
int32_t randPickCard(void);
int32_t randPickResource(void);
int32_t randLostResource(int32_t lostResource[5]);
int32_t randAction(void);
int32_t randBuyCard(void);
int32_t randPickPlayer(int blockIdx);
int32_t randRobPlayer(int target);

// setprop.c
int32_t placeNode(int32_t playerID, int32_t nodePos);
int32_t placeRoad(int32_t playerID, int32_t roadPos);
int32_t placeRobberPos(int32_t robberPos);
int32_t gainSettleResource(int32_t playerID, int32_t nodePos);
int32_t gainDiceResource(void);
int32_t updateCard(void);
int32_t gainMonopolyResource(int32_t resType);
int32_t placeBankTrade(int32_t outRes, int32_t inRes);
int32_t discardResource(int32_t playerID, int32_t lostRes[6]);

// check.c
int32_t checkNode(int32_t playerID, int32_t nodePos);
int32_t checkRoad(int32_t playerID, int32_t roadPos);
int32_t checkBuyCard(int32_t playerID);
int32_t checkUseCard(int32_t playerID, int32_t cardID);
int32_t checkBankTrade(int32_t playerID, int32_t giveRes, int32_t takeRes);
int32_t checkDiscard(int32_t playerID, int32_t selectCard[6]);
int32_t checkRobberPos(int32_t blockPos);
int32_t checkRobbable(int32_t playerID, int32_t blockPos);
int32_t checkRobAct(int32_t robPlayer, int32_t blockPos);

//screen.c
int display();
Display* create_display();
void update_display(Display* display);
void destroy_display(Display* display);
int event_loop(Display* display);
void renderBlocks(Display* display);
void renderNodesAndVillage(Display* display);
void renderRoads(Display* display);

// update.c
int32_t updateScore(void);

// debug.c
void printGraph(void);
