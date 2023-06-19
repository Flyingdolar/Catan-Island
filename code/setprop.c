#include "catan.h"

// 1. 建造村莊、城市
int32_t placeNode(int32_t playerID, int32_t nodePos) {
    pPlayer player = &game->player[playerID];
    pList nodeList;
    pNode buildNode;

    nodeList = getNode(game->node, nodePos);
    if (nodeList == NULL) {
        PRINTD("無法取得節點\n");
        return -1;
    }
    buildNode = entry(nodeList, sNode);
    PRINTL("正在座標 %d, %d 的地點建造村莊，等級為 %d", buildNode->coord[0], buildNode->coord[1], buildNode->building + 1);
    buildNode->owner = playerID, buildNode->building++;

    if (buildNode->harbor != NULL) {  // 如果建造的節點有港口，則將港口的資源加入玩家資源
        PRINTL("正在將港口資源 %d 加入玩家資源", buildNode->harbor->type);
        player->harbor[buildNode->harbor->type]++;
        buildNode->harbor->owner = playerID;
    }

    PRINTL("正在放置村落、城市標記於節點上");
    if (buildNode->building == VILLAGE) player->building[VILLAGE]++;                         // 建造村莊: 場上村莊數量加一
    if (buildNode->building == CITY) player->building[CITY]++, player->building[VILLAGE]--;  // 建造城市: 場上城市數量加一，村莊數量減一
    for (int32_t idx = 1; idx <= 2; idx++) {
        PRINTL("玩家 %d 建築物 %d 場上數量 %d", playerID, idx, player->building[idx]);
    }
    // 建築階段: 建造村落需要扣除資源
    if (game->state == BUILD && buildNode->building == VILLAGE) {
        PRINTL("正在扣除建築村莊所需資源");
        player->resource[WOOD]--, player->resource[BRICK]--;
        player->resource[SHEEP]--, player->resource[WHEAT]--;
        for (int32_t idx = 1; idx <= 5; idx++) {
            PRINTL("玩家 %d 資源 %d 場上數量 %d", playerID, idx, player->resource[idx]);
        }
    }
    // 建築階段: 建造城市需要扣除資源
    if (game->state == BUILD && buildNode->building == CITY) {
        PRINTL("正在扣除建築程式所需資源");
        player->resource[ORE] -= 3, player->resource[WHEAT] -= 2;
        for (int32_t idx = 1; idx <= 5; idx++) {
            PRINTL("玩家 %d 資源 %d 場上數量 %d", playerID, idx, player->resource[idx]);
        }
    }
    // 更新玩家分數
    updateScore();
    return 0;
}

// 2. 建造道路
int32_t placeRoad(int32_t playerID, int32_t roadPos) {
    pPlayer player = &game->player[playerID];
    pList roadList;
    pRoad buildRoad;

    roadList = getNode(game->road, roadPos);
    if (roadList == NULL) {
        PRINTD("無法取得道路\n");
        return -1;
    }
    buildRoad = entry(roadList, sRoad);
    PRINTL("正在座標 %d, %d 的地點建造道路", buildRoad->coord[0], buildRoad->coord[1]);
    buildRoad->owner = playerID;

    PRINTL("正在放置道路標記於道路上");
    player->building[ROAD]++;  // 記錄玩家場上道路數量
    PRINTL("玩家 %d 建築物 %d 剩餘 %d", playerID, ROAD, player->building[ROAD]);
    if (15 - player->building[ROAD] < 0) {
        PRINTD("沒有足夠建築物建造道路\n");
        return -1;
    }

    if (game->state == BUILD) {  // 建築階段需要扣除資源
        PRINTL("正在扣除建築道路所需資源");
        player->resource[WOOD]--, player->resource[BRICK]--;
        for (int32_t idx = 1; idx <= 5; idx++) {
            PRINTL("玩家 %d 資源 %d 場上數量 %d", playerID, idx, player->resource[idx]);
        }
    }
    // 更新玩家分數
    updateScore();
    return 0;
}

// 3. 放置盜賊
int32_t placeRobberPos(int32_t robberPos) {
    pList blockList;
    pBlock placeBlock;

    blockList = getNode(game->block, robberPos);
    if (blockList == NULL) {
        PRINTD("無法取得目標板塊\n");
        return -1;
    }
    placeBlock = entry(blockList, sBlock);
    PRINTL("正在座標 %d, %d 的地點放置盜賊", placeBlock->coord[0], placeBlock->coord[1]);
    game->robber = placeBlock;

    // 更新玩家分數
    // updateScore();
    return 0;
}

// 4. 獲得資源 (SETTLE 階段)
int32_t gainSettleResource(int32_t playerID, int32_t nodePos) {
    pNode gainNode = entry(getNode(game->node, nodePos), sNode);
    int resType;

    if (gainNode == NULL) {
        PRINTD("無法取得節點\n");
        return -1;
    }
    for (int32_t idx = 0; idx < 3; idx++) {
        if (gainNode->block[idx] == NULL) continue;
        resType = gainNode->block[idx]->resource;
        PRINTL("正在獲得資源 %d", resType);
        game->player[playerID].resource[resType]++;
        game->player[NONE].resource[resType]--;
        game->player[playerID].resource[ALL]++;
        game->player[NONE].resource[ALL]--;
    }
    return 0;
}

// 5. 獲得資源 (DICE 階段)
int32_t gainDiceResource(void) {
    int32_t diceSum = game->dice[0] + game->dice[1];

    forList(game->block, blockList) {
        pBlock block = entry(blockList, sBlock);
        // 沒有骰到的板塊不會發放資源
        if (block->number != diceSum) continue;
        // 被盜賊擋住的板塊不會發放資源
        if (block->list.index == game->robber->list.index) continue;
        for (int32_t idx = 0; idx < 6; idx++) {
            if (block->node[idx] == NULL) continue;
            if (block->node[idx]->owner == NONE) continue;
            int32_t playerID = block->node[idx]->owner;
            int32_t resType = block->resource;
            PRINTL("正在獲得資源 %d", resType);
            int32_t gain = block->node[idx]->building == CITY ? 2 : 1;
            game->player[playerID].resource[resType] += gain;
            game->player[NONE].resource[resType] -= gain;
            game->player[playerID].resource[ALL] += gain;
            game->player[NONE].resource[ALL] -= gain;
        }
    }
    return 0;
}

// 6. 更新卡片狀態
int32_t updateCard(void) {
    for (int32_t idx = 1; idx <= 4; idx++) {
        forList(game->player[idx].devcard, cardList) {
            pDevcard card = entry(cardList, sDevcard);
            if (card->status == UNUSED)
                card->status = AVAILABLE;
        }
    }
    return 0;
}

// 7. 壟斷資源
int32_t gainMonopolyResource(int32_t resType) {
    int32_t sumRes = 0;
    for (int32_t idx = 1; idx <= 4; idx++) {
        sumRes += game->player[idx].resource[resType];
        game->player[idx].resource[ALL] -= game->player[idx].resource[resType];
        game->player[idx].resource[resType] = 0;
    }
    game->player[game->turn].resource[resType] = sumRes;
    game->player[game->turn].resource[ALL] += sumRes;
    return 0;
}

// 8. 銀行交易
int32_t placeBankTrade(int32_t outRes, int32_t inRes) {
    int32_t require = 4;

    if (game->player[game->turn].harbor[ALL] > 0) require = 3;
    if (game->player[game->turn].harbor[outRes] > 0) require = 2;

    game->player[game->turn].resource[outRes] -= require;
    game->player[NONE].resource[outRes] += require;
    game->player[game->turn].resource[inRes]++;
    game->player[NONE].resource[inRes]--;

    game->player[game->turn].resource[ALL] -= require - 1;
    game->player[NONE].resource[ALL] += require - 1;
    return 0;
}

// 9. 丟棄資源
int32_t discardResource(int32_t playerID, int32_t lostRes[6]) {
    for (int32_t idx = 0; idx < 6; idx++) {
        game->player[playerID].resource[idx] -= lostRes[idx];
        game->player[NONE].resource[idx] += lostRes[idx];
    }
    return 0;
}
