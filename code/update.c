#include "catan.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct _sPath_ *pPath;
typedef struct _sPath_ {
    pRoad road;
    sList list;
} sPath;

bool visited[73];  // 紀錄道路是否走訪過

int32_t updateScore(void) {
    pPlayer plyr = game->player;
    // 更新每個玩家的分數
    for (int32_t idx = 1; idx <= 4; idx++) {
        plyr[idx].score = 0, plyr[idx].armySize = 0, plyr[idx].roadlength = 0;          // 將分數、軍隊大小、道路長度歸零
        plyr[idx].score += plyr[idx].building[VILLAGE] + plyr[idx].building[CITY] * 2;  // 計算建築物分數
        forList(plyr[idx].devcard, cardList) {
            pDevcard card = entry(cardList, sDevcard);
            if (card->type == VICTORY_POINT) plyr[idx].score++;                      // 計算分數卡分數
            if (card->type == KNIGHT && card->status == USED) plyr[idx].armySize++;  // 計算軍隊大小
        }
        if (plyr[idx].armySize >= 3 && plyr[idx].armySize > game->armyKing.size)
            game->armyKing.size = plyr[idx].armySize, game->armyKing.owner = idx;  // 更新軍隊王
    }
    findLongestRoad();                                                        // 更新道路王與每個玩家的道路長度
    if (game->armyKing.owner != NONE) plyr[game->armyKing.owner].score += 2;  // 軍隊王額外加兩分
    if (game->roadKing.owner != NONE) plyr[game->roadKing.owner].score += 2;  // 道路王額外加兩分

    // Check if anyone has won the game
    for (int32_t idx = 1; idx <= 4; idx++)
        if (plyr[idx].score >= 10) game->state = END;
    return 0;
}

int32_t findLongestRoad() {
    pList edge = initList();  // 找出道路邊界，並將其記錄起來

    forList(game->road, roadList) {
        pRoad road = entry(roadList, sRoad);
        if (road->owner == NONE) continue;
        memset(visited, 0, sizeof(visited));
        game->player[road->owner].roadlength = MAX(game->player[road->owner].roadlength, dfs(road, road->owner));
    }
    // 找出最長道路的玩家
    for (int32_t idx = 1; idx <= 4; idx++) {
        if (game->player[idx].roadlength < 5) continue;
        if (game->player[idx].roadlength > game->roadKing.size) {
            game->roadKing.size = game->player[idx].roadlength;
            game->roadKing.owner = idx;
        }
    }
    return 0;
}

int32_t dfs(pRoad road, int32_t owner) {
    int32_t len = 1, left = 0, right = 0;

    if (road->owner != owner) return 0;
    if (visited[road->list.index]) return 0;
    visited[road->list.index] = true;
    // 找左邊的最長路徑
    if (road->node[0]->owner == NONE || road->node[0]->owner == owner) {
        for (int32_t idx = 0; idx < 3; idx++) {
            if (road->node[0]->road[idx] == road) continue;
            if (road->node[0]->road[idx] == NULL) continue;
            left = MAX(left, dfs(road->node[0]->road[idx], owner));
            len += left;
        }
    }
    // 找右邊的最長路徑
    if (road->node[1]->owner == NONE || road->node[1]->owner == owner) {
        for (int32_t idx = 0; idx < 3; idx++) {
            if (road->node[1]->road[idx] == road) continue;
            if (road->node[1]->road[idx] == NULL) continue;
            right = MAX(right, dfs(road->node[1]->road[idx], owner));
            len += right;
        }
    }
    return len;
}
