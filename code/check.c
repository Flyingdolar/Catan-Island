#include "catan.h"

// 1. 檢查節點是否可以建造村莊、城市
int32_t checkNode(int32_t playerID, int32_t nodePos) {
    pNode buildNode = entry(getNode(game->node, nodePos), sNode);

    // SETTLE 階段不可以建造城市
    if (game->state == SETTLE && buildNode->owner == playerID) {
        PRINTC(YELLOW, "輸入錯誤 - SETTLE 階段不可以建造城市\n");
        return -1;
    }

    // 確認節點是否已經有其他玩家的建築物
    if (buildNode->owner != playerID && buildNode->owner != NONE) {
        PRINTC(YELLOW, "輸入錯誤 - 節點已經有其他玩家的建築物\n");
        return -1;
    }

    // 確認節點是否有相鄰的道路
    if (game->state != SETTLE) {
        bool adjRoad = false;
        for (int32_t idx = 0; idx < 3; idx++) {
            if (buildNode->road[idx]->owner == playerID) {
                adjRoad = true;
                break;
            }
        }
        if (!adjRoad) {
            PRINTC(YELLOW, "輸入錯誤 - 節點沒有相鄰的道路\n");
            return -1;
        }
    }

    // 確認節點是否有相鄰的建築物
    bool adjNode = false;
    for (int32_t idx = 0; idx < 3; idx++) {
        if (buildNode->node[idx]->owner != NONE) {
            adjNode = true;
            break;
        }
    }
    if (adjNode) {
        PRINTC(YELLOW, "輸入錯誤 - 節點有相鄰的建築物\n");
        return -1;
    }

    // 確認玩家是否有足夠的資源 - 村莊
    if (game->state != SETTLE && buildNode->building == NONE) {
        bool enough = true;
        if (game->player[playerID].resource[WOOD] < 1) enough = false;
        if (game->player[playerID].resource[BRICK] < 1) enough = false;
        if (game->player[playerID].resource[SHEEP] < 1) enough = false;
        if (game->player[playerID].resource[WHEAT] < 1) enough = false;
        if (!enough) {
            PRINTC(YELLOW, "輸入錯誤 - 資源不足\n");
            return -1;
        }
    }

    // 確認玩家是否有足夠的資源 - 城市
    if (game->state != SETTLE && buildNode->building == VILLAGE) {
        bool enough = true;
        if (game->player[playerID].resource[ORE] < 3) enough = false;
        if (game->player[playerID].resource[WHEAT] < 2) enough = false;
        if (!enough) {
            PRINTC(YELLOW, "輸入錯誤 - 資源不足\n");
            return -1;
        }
    }

    // 合法的建築物節點
    return 0;
}

// 2. 檢查道路是否可以建造
int32_t checkRoad(int32_t playerID, int32_t roadPos) {
    pRoad buildRoad = entry(getNode(game->road, roadPos), sRoad);

    // 確認道路是否已佔據
    if (buildRoad->owner != NONE) {
        PRINTC(YELLOW, "輸入錯誤 - 道路已經有其他玩家佔據\n");
        return -1;
    }

    // 確認道路是否有相鄰的道路、建築物
    bool adjObject = false;
    for (int32_t ndx = 0; ndx < 2; ndx++) {
        if (buildRoad->node[ndx]->owner == playerID) {
            adjObject = true;
            break;
        }
        for (int32_t idx = 0; idx < 3; idx++) {
            if (buildRoad->node[ndx]->road[idx]->owner == playerID) {
                adjObject = true;
                break;
            }
        }
        if (adjObject) break;
    }
    if (!adjObject) {
        PRINTC(YELLOW, "輸入錯誤 - 道路沒有相鄰的道路、建築物\n");
        return -1;
    }

    // 確認玩家是否有足夠的資源
    if (game->state != SETTLE) {
        bool enough = true;
        if (game->player[playerID].resource[WOOD] < 1) enough = false;
        if (game->player[playerID].resource[BRICK] < 1) enough = false;
        if (!enough) {
            PRINTC(YELLOW, "輸入錯誤 - 資源不足\n");
            return -1;
        }
    }

    // 合法的建造道路
    return 0;
}

// 3. 檢查是否可以購買開發卡
int32_t checkBuyCard(int32_t playerID) {
    // 確認牌庫裏面還有牌
    if (game->player[NONE].devcard->index == 0) {
        PRINTC(YELLOW, "輸入錯誤 - 牌庫裏面沒有牌了\n");
        return -1;
    }

    // 確認玩家是否有足夠的資源
    bool enough = true;
    if (game->player[playerID].resource[ORE] < 1) enough = false;
    if (game->player[playerID].resource[SHEEP] < 1) enough = false;
    if (game->player[playerID].resource[WHEAT] < 1) enough = false;
    if (!enough) {
        PRINTC(YELLOW, "輸入錯誤 - 資源不足\n");
        return -1;
    }

    // 合法的購買開發卡
    return 0;
}

// 4. 檢查是否可以使用開發卡
int checkUseCard(int32_t player, int32_t cardID) {
    pList cardHead = game->player[player].devcard;

    // 確認玩家是否有該張開發卡
    if (cardHead->index <= (size_t)cardID || cardID < 0) {
        PRINTC(YELLOW, "輸入錯誤 - 玩家沒有該張開發卡\n");
        return -1;
    }

    // 確認該卡片為可以使用的狀態
    pDevcard card = entry(getNode(cardHead, cardID), sDevcard);
    if (card->status != AVAILABLE) {
        PRINTC(YELLOW, "輸入錯誤 - 該卡片不是可以使用的狀態\n");
        return -1;
    }

    // 合法的使用開發卡
    return 0;
}

// 5. 檢查銀行交易是否合法
int32_t checkBankTrade(int32_t playerID, int32_t giveRes, int32_t takeRes) {
    int32_t require = 4;

    // 檢查不合法輸入
    if (giveRes == takeRes) {
        PRINTC(YELLOW, "輸入錯誤 - 資源不可相同\n");
        return -1;
    }
    if (giveRes < 1 || giveRes > 5 || takeRes < 1 || takeRes > 5) {
        PRINTC(YELLOW, "輸入錯誤 - 未知的資源種類\n");
        return -1;
    }

    // 設置好港口優惠
    if (game->player[playerID].harbor[0] > 0) require = 3;
    if (game->player[playerID].harbor[giveRes] > 0) require = 2;

    // 確認玩家是否有足夠的資源
    if (game->player[playerID].resource[giveRes] < require) {
        PRINTC(YELLOW, "輸入錯誤 - 資源不足\n");
        return -1;
    }

    // 合法的銀行交易
    return 0;
}

// 6. 檢查捨棄的資源張數是否合法
int32_t checkDiscard(int32_t playerID, int32_t discards, int32_t selectCard[5]) {
    int32_t total = 0, holds;

    // 檢查每種資源的張數是否合法
    for (int32_t ndx = 0; ndx < 5; ndx++) {
        holds = game->player[playerID].resource[ndx + 1];
        if (selectCard[ndx] < 0 || selectCard[ndx] > holds) {
            PRINTC(YELLOW, "輸入錯誤 - 捨棄的資源張數不合法\n");
            return -1;
        }
        total += selectCard[ndx];
    }

    // 檢查捨棄的資源總張數是否合法
    if (total != discards) {
        PRINTC(YELLOW, "輸入錯誤 - 捨棄的資源總張數不相符\n");
        return -1;
    }

    // 合法的捨棄資源
    return 0;
}

// 7. 檢查盜賊移動是否合法
int32_t checkRobber(int32_t playerID, int32_t blockPos) {
    // FIXME
    // 合法的盜賊移動
    return 0;
}

// 8. 檢查玩家是否可以進行盜賊掠奪
int32_t checkRobPlayer(int32_t playerID, int32_t blockPos) {
    pBlock block = entry(getNode(game->block, blockPos), sBlock);
    bool hasTown = false;

    // 確認該地是否有其他玩家的城鎮或村莊
    for (int idx = 0; idx < 6; idx++) {
        if (block->node[idx]->owner == game->turn) continue;
        if (block->node[idx]->owner != 0 && block->node[idx]->owner != playerID) {
            hasTown = true;
            break;
        }
    }

    // 沒有其他玩家的城鎮或村莊
    if (!hasTown) {
        PRINTL("此地沒有其他玩家的城鎮或村莊");
        return -1;
    }
    return 0;
}

// 9. 檢查玩家掠奪行動是否合法
int32_t checkRobAct(int32_t robPlayer, int32_t blockPos) {
    pBlock block = entry(getNode(game->block, blockPos), sBlock);
    bool hasTown = false;

    // 確認該地是否有該玩家的城鎮或村莊
    for (int idx = 0; idx < 6; idx++) {
        if (block->node[idx]->owner == robPlayer) {
            hasTown = true;
            break;
        }
    }

    // 沒有該玩家的城鎮或村莊
    if (!hasTown) {
        PRINTC(YELLOW, "輸入錯誤 - 無法掠奪此玩家，此地沒有該玩家的城鎮或村莊\n");
        return -1;
    }

    // 合法的掠奪行動
    return 0;
}
