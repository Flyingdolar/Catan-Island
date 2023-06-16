#pragma once
// Include Standard Library
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
pGame game;

// Function Declaration

// init.c
int32_t init(void);
int32_t init_player(void);
int32_t init_block(void);
int32_t init_node(void);
int32_t init_road(void);
int32_t init_harbor(void);
int32_t init_map(void);
int32_t init_randomize(void);
