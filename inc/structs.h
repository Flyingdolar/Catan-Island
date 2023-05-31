#pragma once
#include "../code/catan.h"

#define NONE -1  // 無 (用於 src, user, cType)

typedef enum _source_ {  // 資源
    WOOD = 0,            // --木頭 (港口 2:1)
    BRICK = 1,           // --磚塊 (港口 2:1)
    SHEEP = 2,           // --羊毛 (港口 2:1)
    WHEAT = 3,           // --小麥 (港口 2:1)
    STONE = 4,           // --石頭 (港口 2:1)
    DESERT = 5,          // --沙漠
    ALL = 5,             // --全部 (港口 3:1)
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
typedef struct _sBuild_* pBuild;
typedef struct _sHarbor_* pHarbor;
typedef struct _sRoad_* pRoad;
typedef struct _sCard_* pCard;
typedef struct _sPlayer_* pPlayer;
typedef struct _sGame_* pGame;

typedef struct _sBlock_ {  // 板塊
    uint8_t id;            // --板塊編號
    src source;            // --板塊資源
    uint8_t number;        // --板塊數字（骰子）
    pBuild BUILD[6];       // --板塊上的建築物 [0]: 左上、[1]: 右上、[2]: 下、[3]: 左下、[4]: 右下、[5]: 中間
    pBlock NEIGHBOR[6];    // --鄰近的板塊 [0]: 上、[1]: 右上、[2]: 右下、[3]: 下、[4]: 左下、[5]: 左上
    sList list;
} sBlock;

typedef struct _sBuild_ {  // 建築物
    uint8_t id;            // --建築物編號
    user owner;            // --建築物擁有者
    pBlock BLOCK[3];       // --建築物三個角落的板塊
    pRoad ROAD[3];         // --建築物三條邊的道路
    pHarbor HARBOR;        // --建築物的港口 (若有)
    sList list;
} sBuild;

typedef struct _sHarbor_ {  // 港口
    uint8_t id;             // --港口編號
    src source;             // --港口資源
    pBuild BUILD[2];        // --港口所在的建築物
    sList list;
} sHarbor;

typedef struct _sRoad_ {  // 道路
    uint8_t id;           // --道路編號
    user owner;           // --道路擁有者
    pBuild BUILD[2];      // --道路兩端的建築物
    sList list;
} sRoad;

typedef struct _sCard_ {  // 卡片
    cType func;           // --卡片功能、類型
    cState state;         // --卡片狀態
    pCard PREV, NEXT;     // --上下一張卡片
} sCard;

typedef struct _sPlayer_ {      // 玩家
    uint8_t score;              // --玩家分數
    uint8_t source[5];          // --玩家持有資源
    pList village, city, road;  // --玩家村莊、城市、道路
    pList cards;                // --玩家持有卡片
    sList list;
} sPlayer;

typedef struct _sKing_ {  // 王（最長道路、最大軍隊）
    user owner;           // --持有玩家
    uint8_t count;        // --持有數量
} sKing;

typedef struct _sGame_ {       // 遊戲
    uint8_t round, turn;       // --回合、輪到誰
    uint8_t dice;              // --骰子點數
    uint8_t source[5];         // --資源牌
    pBlock blocks[19];         // --板塊
    pBuild builds[54];         // --建築物
    pHarbor harbors[9];        // --港口
    pRoad roads[72];           // --道路
    pList players;             // --玩家
    pList cards;               // --卡片
    pBlock robber;             // --強盜
    sKing armyKing, roadKing;  // --最大軍隊、最長道路
} sGame;
