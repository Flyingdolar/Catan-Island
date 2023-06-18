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
// Developer Mode
#define DEV() if (true)
#else
#define PRINTD(...)
#define PRINTL(...)
#define DEV() if (false)
#endif

// Define Color
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define PURPLE 35
#define CYAN 36

// Clear Terminal
#define CLEAR() printf("\033[H\033[2J");

// forever
#define FOREVER(count) for (int count = 0;; count++)

// SET global message
#define setMsg(...) memset(msg, 0, sizeof(msg)), snprintf(msg, sizeof(msg), __VA_ARGS__);

// Print in different color
#define PRINTC(color, ...) printf("\033[%dm", color), \
                           printf(__VA_ARGS__),       \
                           printf("\033[0m")

// readCMD 沒有參數的簡寫方式
#define NO_ARG ((void*)0), 0, 10000, 0

// 類型
#define T_BLOCK 0  // 區塊
#define T_CARD 1   // 發展卡
#define T_BUILD 2  // 建築物
#define T_ROAD 3   // 道路

// 建築指令
#define END_TURN 0    // 結束回合
#define BUILD_ROAD 1  // 建造道路
#define BUILD_NODE 2  // 建造村莊
#define BUY_CARD 3    // 購買發展卡
#define USE_CARD 4    // 使用發展卡
#define TRADE_N 5     // 與銀行交易

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
