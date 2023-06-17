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
    PRINTL("正在座標 %d, %d 的地點建造村莊，等級為 %d\n", buildNode->coord[0], buildNode->coord[1], buildNode->building + 1);
    buildNode->owner = playerID, buildNode->building++;

    if (buildNode->harbor != NULL) {  // 如果建造的節點有港口，則將港口的資源加入玩家資源
        PRINTL("正在將港口資源 %d 加入玩家資源\n", buildNode->harbor->type);
        player->harbor[buildNode->harbor->type]++;
        buildNode->harbor->owner = playerID;
    }

    PRINTL("正在放置村落、城市標記於節點上");
    if (buildNode->building == VILLAGE) player->building[VILLAGE]--;                         // 建造村莊: 減少村莊可建造數
    if (buildNode->building == CITY) player->building[CITY]--, player->building[VILLAGE]++;  // 建造城市: 減少城市可建造數、增加村莊可建造數
    for (int32_t idx = 1; idx <= 2; idx++) {
        PRINTL("玩家 %d 建築物 %d 剩餘 %d", playerID, idx, player->building[idx]);
        if (player->building[idx] < 0) {
            PRINTD("沒有足夠建築物建造村莊\n");
            return -1;
        }
    }
    // 建築階段: 建造村落需要扣除資源
    if (game->state == BUILD && buildNode->building == VILLAGE) {
        PRINTL("正在扣除建築村莊所需資源");
        player->resource[WOOD]--, player->resource[BRICK]--;
        player->resource[SHEEP]--, player->resource[WHEAT]--;
        for (int32_t idx = 1; idx <= 5; idx++) {
            PRINTL("玩家 %d 資源 %d 剩餘 %d", playerID, idx, player->resource[idx]);
            if (player->resource[idx] < 0) {
                PRINTD("沒有足夠資源建造村莊\n");
                return -1;
            }
        }
    }
    // 建築階段: 建造城市需要扣除資源
    if (game->state == BUILD && buildNode->building == CITY) {
        PRINTL("正在扣除建築程式所需資源");
        player->resource[ORE] -= 3, player->resource[WHEAT] -= 2;
        for (int32_t idx = 1; idx <= 5; idx++) {
            PRINTL("玩家 %d 資源 %d 剩餘 %d", playerID, idx, player->resource[idx]);
            if (player->resource[idx] < 0) {
                PRINTD("沒有足夠資源建造村莊\n");
                return -1;
            }
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
    PRINTL("正在座標 %d, %d 的地點建造道路\n", buildRoad->coord[0], buildRoad->coord[1]);
    buildRoad->owner = playerID;

    PRINTL("正在放置道路標記於道路上");
    player->building[ROAD]--;  // 減少道路可建造數
    PRINTL("玩家 %d 建築物 %d 剩餘 %d", playerID, ROAD, player->building[ROAD]);
    if (player->building[ROAD] < 0) {
        PRINTD("沒有足夠建築物建造道路\n");
        return -1;
    }

    if (game->state == BUILD) {  // 建築階段需要扣除資源
        PRINTL("正在扣除建築道路所需資源");
        player->resource[WOOD]--, player->resource[BRICK]--;
        for (int32_t idx = 1; idx <= 5; idx++) {
            PRINTL("玩家 %d 資源 %d 剩餘 %d", playerID, idx, player->resource[idx]);
            if (player->resource[idx] < 0) {
                PRINTD("沒有足夠資源建造村莊\n");
                return -1;
            }
        }
    }
    // 更新玩家分數
    updateScore();
    return 0;
}

// 3. 放置盜賊
int32_t placeRobber(int32_t playerID, int32_t robberPos) {
    pPlayer player = &game->player[playerID];
    pList blockList;
    pBlock placeBlock;

    blockList = getNode(game->block, robberPos);
    if (blockList == NULL) {
        PRINTD("無法取得目標板塊\n");
        return -1;
    }
    placeBlock = entry(blockList, sBlock);
    PRINTL("正在座標 %d, %d 的地點放置盜賊\n", buildRobber->coord[0], buildRobber->coord[1]);
    // FIXME: 放置盜賊

    // 更新玩家分數
    updateScore();
    return 0;
}
