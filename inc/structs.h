#pragma once
#include "../code/catan.h"

#define NONE 0  // 無 

/*
eState: 遊戲狀態 完成後跳到下一個狀態(不一定照順序)
0: STATE_START，初始化
1: STATE_SETTLE，玩家依序放置兩個村莊
2: STATE_DICE，這個狀態下玩家可以做的事情有
    使用發展卡 (騎士卡、x2道路卡*)
    擲骰子 依點數給/棄資源牌
3: STATE_BUILD，這個狀態下玩家可以做的事情有
    建設道路*
    建設村莊*
    購買發展卡 (分數卡*)
    使用發展卡 (騎士卡、x2道路卡*)
    交易(暫時只用銀行交易)
  按下結束回合按鈕後會跳到2 (STATE_DICE)
  *: 這些動作做完要檢查是否勝利條件，如果達成則跳到5 (STATE_END)
4. STATE_ROBBER，這個狀態下要做的事情
    移動強盜
    搶奪其中一位玩家資源
  完成後會跳到3 (STATE_BUILD)
5: STATE_END，遊戲結束，顯示勝利者
*/
typedef enum _game_state_ {
    START   = 0,
    SETTLE  = 1,
    DICE    = 2,
    BUILD   = 3,
    ROBBER  = 4,
    END     = 5,
} eState;

typedef struct _sBlock_*   pBlock;
typedef struct _sNode_*    pNode;
typedef struct _sHarbor_*  pHarbor;
typedef struct _sRoad_*    pRoad;
typedef struct _sDevcard_* pDevcard;
typedef struct _sPlayer_*  pPlayer;
typedef struct _sGame_*    pGame;

typedef struct _sDevcard_ {  // 發展卡
    uint8_t type;            // --種類
    uint8_t status;          // --卡片狀態 (0: unused 1:avaiable 2:used)
    sList   list;
} sDevcard;

typedef struct _sNode_ {   // 節點
    uint8_t coord[2];      // --坐標 0~5 (row)、0~10 (col)
    uint8_t owner;         // --節點擁有者 (0: 無人、1~4: 玩家)
    uint8_t building;      // --建築物種類 (0: 無、1: 村莊、2: 城市)
    pBlock  block[3];      // --節點三個角落的板塊
    pNode   node[3];       // --節點三個角落的節點
    pRoad   road[3];       // --節點三條邊的道路
    pHarbor harbor;        // --節點的港口 (若有)
    sList   list;
} sNode;

typedef struct _sRoad_ {   // 道路
    uint8_t coord[2];      // --坐標 0~10 (row even: 橫的道路;odd: 直的道路)、0~9 (col)
    uint8_t owner;         // --道路擁有者 (0: 無人、1~4: 玩家)
    pNode   node[2];       // --道路兩端的節點
    sList   list;
} sRoad;

typedef struct _sBlock_ {  // 板塊
    uint8_t   coord[2];     // --坐標 0~4 (row)、0~5 (col)
    uint8_t   number;      // --對應骰子 
    uint8_t   resource;      // --對應資源 
    pNode     node[6];     // --板塊上的節點 由正上方開始順時針計算 
    pBlock    block[6];    // --鄰近的板塊 由正上方開始順時針計算
    sList     list;
} sBlock;

typedef struct _sHarbor_ { // 港口
    uint8_t owner;         // --擁有者 (0: 無人、1~4: 玩家)
    uint8_t type;          // --港口種類 (0: 三換一、1~5: 對應資源二換一)
    pNode   node[2];       // --港口所在的節點
    sList   list;
} sHarbor;

typedef struct _sPlayer_ { // 玩家
    uint8_t  id;           // --編號 0: 無人(銀行)、1~4: 玩家
    uint8_t  score;        // --玩家分數
    uint8_t *resource;     // --玩家資源 大小為5+1的陣列，分別對應五種資源 第一項為總數
    uint8_t *building;     // --玩家建築物 大小為3的陣列，分別對應三種建築物
    uint8_t *harbor;       // --玩家港口 大小為6的陣列，分別對應六種港口。0表示沒有擁有
    uint8_t  roadlength;   // --玩家道路數量
    uint8_t  armySize;     // --玩家軍隊數量
    pList    devcard;      // --玩家發展卡
} sPlayer;

typedef struct _sking_ { //最長路、最大騎士
    uint8_t  owner;      // --玩家編號 0: 無人、1~4: 玩家
    uint8_t  size;       // --數量
} sKing;

typedef struct _sGame_{ // 遊戲
    eState  state    ;  // --遊戲狀態 0~5
    uint8_t turn     ;  // --輪到誰 1~4
    uint8_t dice[2]  ;  // --骰子點數 2個1~6
    pPlayer player   ;  // --玩家 0: 無人(銀行)、1~4: 玩家
    pList block      ;  // --板塊的list head
    pList node       ;  // --節點的list head
    pList road       ;  // --道路的list head
    pList harbor     ;  // --港口的list head
    pBlock robber    ;  // --強盜所在的板塊
    sKing roadKing   ;  // --最長道路
    sKing armyKing   ;  // --最大騎士
} sGame;
