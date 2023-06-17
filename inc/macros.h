#pragma once
#include "../code/catan.h"

#ifdef DEBUG
// Print Debug Message
#define PRINTD(...)                                            \
    PRINTC(RED, "%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
    PRINTC(RED, __VA_ARGS__);                                  \
    PRINTC(RED, "\n");
// Print Log Message
#define PRINTL(...)                                              \
    PRINTC(GREEN, "%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
    PRINTC(GREEN, __VA_ARGS__);                                  \
    PRINTC(GREEN, "\n");

#else
#define PRINTD(...)
#define PRINTL(...)
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

// 資源 (可增加)
#define DESERT 0  // 空   (沙漠)
#define ALL 0     // 全部 (港口 3:1)
#define WOOD 1    // 木頭 (森林)
#define BRICK 2   // 磚塊 (丘陵)
#define SHEEP 3   // 羊   (草原)
#define WHEAT 4   // 小麥 (農田)
#define ORE 5     // 礦石 (山地)

// 發展卡 (可增加)
#define VICTORY_POINT 0   // 分數卡
#define KNIGHT 1          // 騎士卡
#define ROAD_BUILDING 2   // 一次建兩條路
#define YEAR_OF_PLENTY 3  // 一次拿兩種資源
#define MONOPOLY 4        // 一次拿走所有玩家的某一種資源

// 卡片狀態
#define UNUSED 0     // 未使用
#define AVAILABLE 1  // 可使用
#define USED 2       // 已使用

// 建築物 (可增加)
#define ROAD 0     // 道路
#define VILLAGE 1  // 村莊
#define CITY 2     // 城市

// 玩家、擁有者 (可增加)
#define NONE 0  // 無主物件
#define PLAYER1 1
#define PLAYER2 2
#define PLAYER3 3
#define PLAYER4 4
