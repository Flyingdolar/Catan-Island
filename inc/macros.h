#pragma once
#include "../code/catan.h"

#ifdef DEBUG
// Print Debug Message
#define PRINTD(...)                                       \
    printf("%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
    printf(__VA_ARGS__);                                  \
    printf("\n");
#else
#define PRINTD(...)
#endif

// Define Color
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define PURPLE 35
#define CYAN 36

// Print in different color
#define PRINTC(color, ...)     \
    printf("\033[%dm", color); \
    printf(__VA_ARGS__);       \
    printf("\033[0m");
