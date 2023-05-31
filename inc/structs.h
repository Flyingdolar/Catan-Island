#pragma once
#include "../code/catan.h"

enum {
    NONE = -1,  // 無 (用於 src, user, cType)
};

typedef enum _source_ {  // 資源
    WOOD = 0,            // --木頭
    BRICK = 1,           // --磚塊
    SHEEP = 2,           // --羊毛
    WHEAT = 3,           // --小麥
    STONE = 4,           // --石頭
    DESERT = 5,          // --沙漠
} src;

typedef enum _user_ {  // 使用者
    PLAYER1 = 0,       // --玩家1
    PLAYER2 = 1,       // --玩家2 (電腦)
    PLAYER3 = 2,       // --玩家3 (電腦)
    PLAYER4 = 3,       // --玩家4 (電腦)
} user;

typedef enum _cType_ {   // 卡片類型
    KNIGHT = 0,          // --騎士
    MONOPOLY = 1,        // --壟斷
    ROAD_BUILDING = 2,   // --建路
    YEAR_OF_PLENTY = 3,  // --豐收
    VICTORY_POINT = 4,   // --勝利點
} cType;

typedef enum _cState_ {  // 卡片狀態
    WAIT = -1,           // --拿到當下，下回合才能使用
    READY = 0,           // --可使用
    USED = 1,            // --已使用
} cState;

typedef struct _sBlock_* pBlock;
typedef struct _sBlock_ {  // 板塊
    src source;            // --板塊資源
    uint8_t number;        // --板塊數字（骰子）
} sBlock;

typedef struct _sBuild_* pBuild;
typedef struct _sBuild_ {  // 建築物
    uint8_t level;         // --建築物等級
    user owner;            // --建築物擁有者
} sBuild;

typedef struct _sRoad_* pRoad;
typedef struct _sRoad_ {  // 道路
    user owner;           // --道路擁有者
} sRoad;

typedef struct _sCard_* pCard;
typedef struct _sCard_ {  // 卡片
    cType type;           // --卡片類型
    cState state;         // --卡片狀態
} sCard;

typedef struct _sPlayer_* pPlayer;
typedef struct _sPlayer_ {  // 玩家
    uint8_t score;          // --玩家分數
    uint8_t resource[5];    // --玩家資源
    pCard cardList;         // --玩家卡片
    pBuild buildList;       // --玩家建築物
    pRoad roadList;         // --玩家道路
} sPlayer;

typedef struct _sRobber_* pRobber;
typedef struct _sRobber_ {  // 強盜
    uint8_t posX, posY;     // --強盜位置
} sRobber;

typedef struct _sKing_* pKing;
typedef struct _sKing_ {  // 王（最長道路、最大軍隊）
    user owner;           // --持有玩家
    uint8_t count;        // --持有數量
} sKing;

typedef struct _sGame_* pGame;
typedef struct _sGame_ {     // 遊戲
    sBlock blockList[5][5];  // --板塊
    sPlayer playerList[4];   // --玩家
    uint8_t round;           // --回合
    uint8_t dice;            // --骰子
    sRobber robber;          // --強盜
    sKing roadKing;          // --最長道路
    sKing armyKing;          // --最大軍隊
} sGame;
