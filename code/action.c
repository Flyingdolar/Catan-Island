#include "catan.h"

// 1 - STATE_SETTLE
int32_t settleAction() {
    int pickNode, pickRoad;

    // 總共進行兩個輪次，每個輪次都會輪到每個玩家放置一個村莊、一條道路
    PRINTL("第一輪回合，順向，放置一個村莊、一條道路");
    // 1. 第一個輪次，玩家順序由 1 開始
    for (int32_t playerIdx = 1; playerIdx <= 4; playerIdx++) {
        // if (index == 1) { ...}  // TODO_S: 文字提示玩家應該要放置村莊
        if (playerIdx != 1) sleep(1);  // 讓電腦等待一秒鐘

        PRINTL("輪到玩家 %d 放置村莊", playerIdx);
        while (1) {  // 直到玩家的操作合法為止
            if (playerIdx == 1) {
                // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
                // pickNode = ....SDL FUNCTION....;
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // pickNode = randPickNode();
            }
            // TODO_F: 使用較寬鬆的規則檢查是否合法（不必相鄰）
            // if (checkVillage(0, pickNode)) break;
        }
        // TODO_F: 設置村莊給該玩家
        // setNode(playerIdx, pickNode);
        // TODO_S: 更新畫面
        // updateMap();

        // if (playerIdx == 1) { ...}  // TODO_S: 文字提示玩家應該要放置道路
        if (playerIdx != 1) sleep(1);  // 讓電腦等待一秒鐘

        PRINTL("輪到玩家 %d 放置道路", playerIdx);
        while (1) {  // 直到玩家的操作合法為止
            if (playerIdx == 1) {
                // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
                // pickRoad = ....SDL FUNCTION....;
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // pickRoad = randPickRoad();
            }
            // TODO_F: 檢查道路選擇是否合法
            // if (checkRoad(0, pickRoad)) break;
        }
        // TODO_F: 放置道路
        // setRoute(playerIdx, pickRoad);
        // TODO_S: 更新畫面
        // updateMap();
    }

    PRINTL("第二輪回合，逆向，放置一個村莊、一條道路，並獲得資源");
    // 2. 第二個輪次，玩家順序由 4 開始
    for (int32_t index = 4; index >= 1; index--) {
        // if (index == 1) { ...}  // TODO_S: 文字提示玩家應該要放置村莊
        if (index != 1) sleep(1);  // 讓電腦等待一秒鐘

        PRINTL("輪到玩家 %d 放置村莊", index);
        while (1) {  // 直到玩家的操作合法為止
            if (index == 1) {
                // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
                // pickNode = ....SDL FUNCTION....;
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // pickNode = randPickNode();
            }
            // TODO_F: 使用較寬鬆的規則檢查是否合法（不必相鄰）
            // if (checkVillage(0, pickNode)) break;
        }
        // TODO_F: 放置村莊、並取得對應資源
        // placeVillage(playerIdx, pickNode);
        // giveSettleResource(playerIdx, pickNode);
        // TODO_S: 更新畫面
        // updateMap();

        // if (index == 1) { ...}  // TODO_S: 文字提示玩家應該要放置道路
        if (index != 1) sleep(1);  // 讓電腦等待一秒鐘

        PRINTL("輪到玩家 %d 放置道路", index);
        while (1) {  // 直到玩家的操作合法為止
            if (index == 1) {
                // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
                // pickRoad = ....
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // pickRoad = randPickRoad();
            }
            // TODO_F: 使用較寬鬆的規則檢查是否合法（不必相鄰）
            // if (checkRoad(0, pickRoad)) break;
        }
        // TODO_F: 放置道路
        // placeRoad(0, pickRoad);
        // TODO_S: 更新畫面
        // updateMap();
    }

    // 3. 遊戲進入正式階段，玩家順序由 1 開始，返回到遊戲主迴圈
    PRINTL("前兩回合結束，遊戲進入正式階段");
    game->state = DICE;  // 遊戲進入擲骰子階段
    game->turn = 1;      // 輪到玩家 1 擲骰子
    return 0;
};

// 2 - STATE_DICE
int32_t diceAction() {
    int diceNumA, diceNumB, sum;

    if (game->turn == 1) {
        // TODO_S: 讓玩家點擊擲骰子按鈕
        // while (...SDL FUNCTION...)
    }
    // TODO_T: 隨機產生一個骰子點數
    // diceNumA = randDiceNum(), diceNumB = randDiceNum();

    game->dice[0] = diceNumA, game->dice[1] = diceNumB;
    sum = diceNumA + diceNumB;
    PRINTL("玩家 %d 擲出 %d 點", game->turn, sum);
    // TODO_S: 更新畫面
    // updateMap();

    if (sum == 7) {
        game->state = ROBBER;
        PRINTL("玩家 %d 擲出 7 點，進入強盜階段", game->turn);
        return 0;
    }

    // TODO_F: 給予玩家資源
    // giveResource(sum);
    game->state = BUILD;
    PRINTL("分發資源給玩家，進入建設階段");
    return 0;
}

// 3 - STATE_BUILD
int32_t buildAction() {
    int32_t userCmd;

    while (1) {
        if (game->turn == 1) {
            // TODO_S: 監聽玩家選擇的建設操作
            // userCmd = ...SDL FUNCTION...
        } else {
            sleep(1);  // 讓電腦等待一秒鐘
            // TODO_T: 讓電腦選擇一個操作
            // userCmd = randAction();
        }
        // 可能操作：
        // 0 - 結束回合, 1 - 建設道路, 2 - 建設村莊, 3 - 購買發展卡
        // 4 - 使用發展卡, 5 - 與銀行交易, 6 - 與玩家交易
        switch (userCmd) {
            case 1:
                PRINTL("玩家 %d 選擇建設道路", game->turn);
                buildRoad();
                break;
            case 2:
                PRINTL("玩家 %d 選擇建設村莊", game->turn);
                buildNode();
                break;
            case 3:
                PRINTL("玩家 %d 選擇購買發展卡", game->turn);
                buyCard();
                break;
            case 4:
                PRINTL("玩家 %d 選擇使用發展卡", game->turn);
                useCard();
                break;
            case 5:
                PRINTL("玩家 %d 選擇與銀行交易", game->turn);
                bankTrade();
                break;
                // case 6:
                //     PRINTL("玩家 %d 選擇與玩家交易", game->turn);
                //     playerTrade();
                //     break;
        }
        if (game->state == END) return 0;  // 有玩家獲得 10 分，遊戲結束
        if (userCmd == 0) break;           // 結束回合
    }
    PRINTL("玩家 %d 結束回合", game->turn);
    game->turn = (game->turn + 1) % 4;
    // TODO_F: 更新卡片為可使用狀態
    // updateCard();
}

// 3.1 - STATE_BUILD_ROAD
int32_t buildRoad() {
    int32_t pickRoad;

    if (game->turn == 1) {
        // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
        // pickRoad = ....
    } else {
        // TODO_T: 讓電腦選擇一個位置
        // pickRoad = randPickRoad();
    }
    PRINTL("玩家 %d 選擇在 %d 建設道路", game->turn, pickRoad);
    // TODO_F: 檢查是否合法
    // if (checkRoad(game->turn, pickRoad)) return 0;
    PRINTL("建設道路合法，進行建設");
    // TODO_F: 放置道路
    // placeRoad(game->turn, pickRoad);
    // TODO_S: 更新畫面
    // updateMap();
    return 0;
}

// 3.2 - STATE_BUILD_NODE
int32_t buildNode() {
    int32_t pickNode;

    if (game->turn == 1) {
        // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
        // pickNode = ....
    } else {
        // TODO_T: 讓電腦選擇一個位置
        // pickNode = randPickNode();
    }
    PRINTL("玩家 %d 選擇在 %d 建設村莊", game->turn, pickNode);
    // TODO_F: 檢查是否合法
    // if (checkNode(game->turn, pickNode)) return 0;
    PRINTL("建設村莊合法，進行建設");
    // TODO_F: 放置村莊
    // placeNode(game->turn, pickNode);
    // TODO_S: 更新畫面
    // updateMap();
    return 0;
}

// 3.3 - STATE_BUY_CARD
int32_t buyCard() {
    // TODO_F: 檢查是否有足夠的資源
    // if (checkResource(game->turn)) return 0;
    PRINTL("玩家 %d 購買發展卡", game->turn);
    // TODO_F: 購買發展卡
    // buyCard(0);
    // TODO_S: 更新畫面
    // updateMap();
    return 0;
}

// 3.4 - STATE_USE_CARD
int32_t useCard() {
    int32_t pickCard;
    int32_t monopolyResource;
    int32_t plentYearResource[2];

    if (game->turn == 1) {
        // TODO_S: 開放發展卡讓玩家點擊，並獲取玩家點擊的位置
        // pickCard = ....
    } else {
        // TODO_T: 讓電腦選擇一個位置
        // pickCard = randPickCard();
    }
    PRINTL("玩家 %d 選擇使用 %d 號發展卡", game->turn, pickCard);
    // TODO_F: 檢查是否合法
    // if (checkCard(game->turn, pickCard)) return 0;
    PRINTL("使用發展卡合法，進行使用");

    pList tmpList = getNode(game->player[game->turn].devcard, pickCard);
    pDevcard cardType = entry(tmpList, sDevcard);

    switch (cardType->type) {
        case KNIGHT:
            PRINTL("玩家 %d 使用騎士卡", game->turn);
            if (game->turn == 1) {
                // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
                // pickNode = ....
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // pickNode = randPickBlock();
            }
            PRINTL("玩家 %d 選擇在 %d 放置強盜", game->turn, pickNode);
            // TODO_F: 更新分數資訊
            // updateScore();
            // TODO_F: 放置強盜
            // placeRobber(pickNode);
            break;
        case ROAD_BUILDING:
            PRINTL("玩家 %d 使用道路建設卡", game->turn);
            for (int32_t idx = 0; idx < 2; idx++) {
                while (1) {
                    if (game->turn == 1) {
                        // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
                        // pickRoad = ....
                    } else {
                        // TODO_T: 讓電腦選擇一個位置
                        // pickRoad = randPickRoad();
                    }
                    PRINTL("玩家 %d 選擇在 %d 建設道路", game->turn, pickRoad);
                    // TODO_F: 檢查是否合法
                    // if (checkRoad(game->turn, pickRoad)) continue;
                    PRINTL("建設道路合法，進行建設");
                    // TODO_F: 放置道路
                    // placeRoad(game->turn, pickRoad);
                    break;
                }
            }
            break;
        case YEAR_OF_PLENTY:
            PRINTL("玩家 %d 使用豐收年卡", game->turn);
            if (game->turn == 1) {
                // TODO_S: 開放資源讓玩家點擊，並獲取玩家點擊的位置
                // plentYearResource = ....
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // plentYearResource[0] = randPickResource();
                // plentYearResource[1] = randPickResource();
            }
            PRINTL("玩家 %d 選擇獲得 %d 資源和 %d 資源", game->turn, plentYearResource[0], plentYearResource[1]);
            // TODO_F: 獲得資源
            // gainPlentyResource(game->turn, plentYearResource[0], plentYearResource[1]);
            break;
        case MONOPOLY:
            PRINTL("玩家 %d 使用獨佔卡", game->turn);
            if (game->turn == 1) {
                // TODO_S: 開放資源讓玩家點擊，並獲取玩家點擊的位置
                // monopolyResource = ....
            } else {
                // TODO_T: 讓電腦選擇一個位置
                // monopolyResource = randPickResource();
            }
            PRINTL("玩家 %d 選擇獨佔 %d 資源", game->turn, pickResource);
            // TODO_F: 獲得資源
            // gainMonopolyResource(game->turn, pickResource);
            break;
    }
    // TODO_S: 更新畫面
    // updateMap();
    return 0;
}

// 3.5 - STATE_BANK_TRADE
int32_t bankTrade() {
    int32_t pickResource[2];  // 要交易的資源：pickResource[0] 是要換出的資源，pickResource[1] 是要獲得的資源

    if (game->turn == 1) {
        // TODO_S: 開放資源讓玩家點擊，並獲取玩家點擊的位置
        // pickResource = ....
    } else {
        // TODO_T: 讓電腦選擇一個位置
        // pickResource[0] = randPickResource();
        // pickResource[1] = randPickResource();
    }
    PRINTL("玩家 %d 選擇以 %d 資源換取 %d 資源", game->turn, pickResource[0], pickResource[1]);
    // TODO_F: 檢查是否合法
    // if (checkBankTrade(game->turn, pickResource[0], pickResource[1])) return 0;
    PRINTL("交易合法，進行交易");
    // TODO_F: 交易
    // bankTrade(game->turn, pickResource[0], pickResource[1]);
    // TODO_S: 更新畫面
    // updateMap();
    return 0;
}

// 3.6 - STATE_PLAYER_TRADE
// 敬請期待未來開發

// 4 - STATE_ROBBER
int32_t robberAction() {
    int32_t pickNode;
    int32_t lostResource[5];

    PRINTL("檢查有無玩家需要丟棄資源");
    for (int32_t playerIdx = 1; playerIdx <= 4; playerIdx++) {
        // TODO_F: 檢查有無玩家需要丟棄資源
        // if (!checkDiscard(playerIdx)) continue;
        PRINTL("玩家 %d 需要丟棄資源", playerIdx);
        if (playerIdx == 1) {
            // TODO_S: 開放資源讓玩家點擊，並獲取玩家點擊的位置
            // lostResource = ....
        } else {
            sleep(1);
            // TODO_T: 讓電腦選擇丟棄資源
            // randLostResource(lostResource);
        }
        // TODO_S: 更新畫面
        // updateMap();
    }

    if (game->turn == 1) {
        // TODO_S: 開放地圖讓玩家點擊，並獲取玩家點擊的位置
        // pickNode = ....
    } else {
        // TODO_T: 讓電腦選擇一個位置
        // pickNode = randPickBlock();
    }
    PRINTL("玩家 %d 選擇在 %d 放置強盜", game->turn, pickNode);
    // TODO_F: 檢查是否合法
    // if (checkRobber(game->turn, pickNode)) return 0;
    PRINTL("放置強盜合法，進行放置");
    // TODO_F: 放置強盜
    // placeRobber(pickNode);
    // TODO_S: 更新畫面
    // updateMap();

    return 0;
}
