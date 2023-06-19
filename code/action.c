#include "catan.h"

// 1 - STATE_SETTLE  
int32_t settleAction() {
    int pickNode, pickRoad;

    PRINTL("第一輪回合，順向，放置一個村莊、一條道路");
    // 1. 第一個輪次，玩家順序由 1 開始
    for (int32_t playerIdx = 1; playerIdx <= 4; playerIdx++) {
        roundCount = 1, game->turn = playerIdx;
        PRINTL("輪到玩家 %d 放置村莊", playerIdx);
        FOREVER(Attempt) {  // 直到玩家的操作合法為止
            if (playerIdx == PLAYER1) {
                // 顯示輸入介面，並獲取玩家輸入的位置
                pickNode = readPos("選擇一個位置建立建築物，請輸入 X Y 座標：", T_BUILD, Attempt);
            } else {
                pickNode = randPickNode();
                PRINTL("玩家 %d 選擇村莊(0-53)：%d", playerIdx, pickNode);
            }
            if (checkNode(playerIdx, pickNode) == -1) continue;
            break;
        }
        placeNode(playerIdx, pickNode);

        printGameInfo(0);
        printf("玩家%d 修建了一個村莊", playerIdx);
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續

        PRINTL("輪到玩家 %d 放置道路", playerIdx);
        FOREVER(Attempt) {  // 直到玩家的操作合法為止
            if (playerIdx == PLAYER1) {
                // 顯示輸入介面，並獲取玩家輸入的位置
                pickRoad = readPos("選擇一個位置建立道路，請輸入 X Y 座標：", T_ROAD, Attempt);
            } else {
                // TODO_T: 讓電腦選擇一個位置
                int randNum = rand() % game->road->index;
                pickRoad = randNum;
            }
            if (checkRoad(playerIdx, pickRoad) == -1) continue;
            break;
        }
        placeRoad(playerIdx, pickRoad);

        printGameInfo(0);
        printf("玩家%d 修建了一條道路", playerIdx);
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
    }

    PRINTL("第二輪回合，逆向，放置一個村莊、一條道路，並獲得資源");
    // 2. 第二個輪次，玩家順序由 4 開始
    for (int32_t playerIdx = 4; playerIdx >= 1; playerIdx--) {
        roundCount = 2, game->turn = playerIdx;
        PRINTL("輪到玩家 %d 放置村莊", playerIdx);
        FOREVER(Attempt) {  // 直到玩家的操作合法為止
            if (playerIdx == 1) {
                // 顯示輸入介面，並獲取玩家輸入的位置
                pickNode = readPos("選擇一個位置建立建築物，請輸入 X Y 座標：", T_BUILD, Attempt);
            } else {
                // TODO_T: 讓電腦選擇一個位置
                pickNode = randPickNode();
            }
            if (checkNode(playerIdx, pickNode) == -1) continue;
            break;
        }
        placeNode(playerIdx, pickNode);
        gainSettleResource(playerIdx, pickNode);

        printGameInfo(0);
        printf("玩家%d 修建了一個村莊，並取得了附近的資源", playerIdx);
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  //  按下 ENTER 繼續

        PRINTL("輪到玩家 %d 放置道路", playerIdx);
        FOREVER(Attempt) {  // 直到玩家的操作合法為止
            if (playerIdx == 1) {
                // 顯示輸入介面，並獲取玩家輸入的位置
                pickRoad = readPos("選擇一個位置建立道路，請輸入 X Y 座標：", T_ROAD, Attempt);
            } else {
                // TODO_T: 讓電腦選擇一個位置
                int randNum = rand() % game->road->index;
                pickRoad = randNum;
            }
            if (checkRoad(playerIdx, pickRoad) == -1) continue;
            break;
        }
        placeRoad(playerIdx, pickRoad);

        printGameInfo(0);
        printf("玩家 %d 修建了一條道路", playerIdx);

        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
    }

    // 3. 遊戲進入正式階段，玩家順序由 1 開始，返回到遊戲主迴圈
    PRINTL("前兩回合結束，遊戲進入正式階段");
    game->state = DICE;              // 遊戲進入擲骰子階段
    game->turn = 1, roundCount = 3;  // 輪到玩家 1 擲骰子
    return 0;
};

// 2 - STATE_DICE
int32_t diceAction() {
    int diceSum;

    printGameInfo(0);
    PRINTL("輪到玩家 %d 擲骰子", game->turn);
    // 按下任意鍵擲骰子
    if (game->turn == PLAYER1) {
        PRINTC(BLUE, ".........[按任意鍵擲骰子]"), readCMD(NO_ARG);
    }

    // 隨機產生一個骰子點數
    randDiceNum();
    diceSum = game->dice[0] + game->dice[1];
    PRINTL("玩家 %d 擲出 %d 點", game->turn, diceSum);

    printGameInfo(0);
    printf("玩家 %d 擲骰擲出了 %d 與 %d ，一共是 %d 點", game->turn, game->dice[0], game->dice[1], diceSum);

    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續

    if (diceSum == 7) {
        game->state = ROBBER;
        PRINTL("玩家 %d 擲出 7 點，進入強盜階段", game->turn);
        return 0;
    }

    PRINTL("分發資源給玩家，進入建設階段");
    gainDiceResource();  // 分發資源給玩家

    printGameInfo(0);
    printf("在 %d 點的位置有建築物的玩家獲得資源", diceSum);

    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續
    game->state = BUILD;
    return 0;
}

// 3 - STATE_BUILD
int32_t buildAction() {
    int32_t userCmd;

    while (1) {
        if (game->turn == PLAYER1) {
            FOREVER(Attempt) {
                // 顯示地圖
                printGameInfo(NONE);
                if (userCmd == -1) {
                    setMsg("輸入錯誤 - 未知的指令，請重新輸入");
                    continue;
                }
                printf("請點擊 [ENTER] 關閉地圖，並選擇建設操作......\n");
                display();  // 等待玩家按下 ENTER
                // 獲取玩家輸入的操作
                PRINTC(GREEN, "請選擇建設操作 >>\n");
                PRINTC(BLUE, ">>>> 建設道路[1] 建設村莊[2] 購買發展卡[3]\n");
                PRINTC(BLUE, ">>>> 使用發展卡[4] 交換資源[5] 顯示地圖[6]\n");
                PRINTC(PURPLE, ">>>>  保存並退出遊戲[7]\n");
                PRINTC(PURPLE, ">>>>  結束回合[0]\n\n");
                userCmd = readCMD("建設操作：", 0, 7, Attempt);
                if (userCmd == 6) continue;
                break;
            }
        } else {
            userCmd = randAction();
        }
        switch (userCmd) {
            case BUILD_ROAD:
                PRINTL("玩家 %d 選擇建設道路", game->turn);
                buildRoad();
                break;
            case BUILD_NODE:
                PRINTL("玩家 %d 選擇建設村莊", game->turn);
                buildNode();
                break;
            case BUY_CARD:
                PRINTL("玩家 %d 選擇購買發展卡", game->turn);
                buyCard();
                break;
            case USE_CARD:
                PRINTL("玩家 %d 選擇使用發展卡", game->turn);
                useCard();
                break;
            case TRADE_N:
                PRINTL("玩家 %d 選擇與銀行交易", game->turn);
                bankTrade();
                break;
        }
        if (game->state == END) return 0;  // 有玩家獲得 10 分，遊戲結束
        if (userCmd == 7) return 1;        // 保存並退出遊戲
        if (userCmd == 0) break;           // 結束回合
    }
    PRINTL("玩家 %d 結束回合", game->turn);
    game->state = DICE, game->turn++;
    if (game->turn > PLAYER4) game->turn = PLAYER1, roundCount++;
    updateCard();
    return 0;
}

// 3.1 - STATE_BUILD_ROAD
int32_t buildRoad() {
    int32_t pickRoad;

    if (game->turn == 1) {
        // 顯示輸入介面，並獲取玩家輸入的位置
        pickRoad = readPos("選擇一個位置建立道路，請輸入 X Y 座標：", T_ROAD, 0 - BUILD_ROAD);
    } else {
        pickRoad = randPickRoad();
    }
    PRINTL("玩家 %d 選擇在 %d 建設道路", game->turn, pickRoad);
    if (checkRoad(game->turn, pickRoad) == -1) {
        printGameInfo(BUILD_ROAD);
        PRINTC(YELLOW, "%s\n", msg);
        PRINTC(YELLOW, "建設道路失敗，請重新選擇");
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
        return -1;
    }
    PRINTL("建設道路合法，進行建設");
    placeRoad(game->turn, pickRoad);
    // TODO_S: 更新畫面
    printGameInfo(BUILD_ROAD);
    printf("玩家 %d 修建了一條道路", game->turn);

    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續
    return 0;
}

// 3.2 - STATE_BUILD_NODE
int32_t buildNode() {
    int32_t pickNode;

    if (game->turn == 1) {
        // 顯示輸入介面，並獲取玩家輸入的位置
        pickNode = readPos("選擇一個位置建立村莊，請輸入 X Y 座標：", T_BUILD, 0 - BUILD_NODE);
    } else {
        // TODO_T: 讓電腦選擇一個位置
        pickNode = randPickNode();
        DEV() {
            printf("玩家 %d 請選擇建設村莊的位置：", game->turn);
            scanf("%d", &pickNode);
        }
    }
    PRINTL("玩家 %d 選擇在 %d 建設村莊", game->turn, pickNode);
    if (checkNode(game->turn, pickNode) == -1) {
        printGameInfo(BUILD_NODE);
        PRINTC(YELLOW, "%s\n", msg);
        PRINTC(YELLOW, "修建村莊失敗，請重新選擇");
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
        return -1;
    }
    PRINTL("建設村莊合法，進行建設");
    placeNode(game->turn, pickNode);
    // TODO_S: 更新畫面
    printGameInfo(BUILD_NODE);
    printf("玩家 %d 修建了一個村莊", game->turn);

    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續
    return 0;
}

// 3.3 - STATE_BUY_CARD
int32_t buyCard() {
    // 檢查是否有足夠的資源
    if (checkBuyCard(game->turn) == -1) {
        printGameInfo(BUY_CARD);
        PRINTC(YELLOW, "%s\n", msg);
        PRINTC(YELLOW, "購買發展卡失敗，請重新選擇");
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
        return -1;
    }
    PRINTL("玩家 %d 購買發展卡", game->turn);
    randBuyCard();
    printf("玩家 %d 購買了一張發展卡", game->turn);
    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續
    return 0;
}

// 3.4 - STATE_USE_CARD
int32_t useCard() {
    int32_t pickCard;
    int32_t resourceType;

    if (game->turn == 1) {
        // 顯示輸入介面，並獲取玩家輸入的位置
        pickCard = readCard("選擇一張發展卡使用，請輸入卡片編號：", 0 - USE_CARD);
    } else {
        // TODO_T: 讓電腦選擇一個位置
        pickCard = randPickCard();
    }
    PRINTL("玩家 %d 選擇使用 %d 號發展卡", game->turn, pickCard);
    // 檢查是否合法
    if (checkUseCard(game->turn, pickCard) == -1) {
        printGameInfo(USE_CARD);
        PRINTC(YELLOW, "%s\n", msg);
        PRINTC(YELLOW, "使用發展卡失敗，請重新選擇");
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
        return -1;
    }
    PRINTL("使用發展卡合法，進行使用");

    pList tmpList = getNode(game->player[game->turn].devcard, pickCard);
    pDevcard cardType = entry(tmpList, sDevcard);

    switch (cardType->type) {
        case KNIGHT:
            PRINTL("玩家 %d 使用騎士卡", game->turn);
            int32_t pickBlock;
            if (game->turn == 1) {
                // 顯示輸入介面，並獲取玩家輸入的位置
                pickBlock = readPos("<使用騎士卡>\n將強盜驅趕到一個新板塊，請輸入 X Y 座標：", T_BLOCK, 0 - USE_CARD);
            } else {
                // TODO_T: 讓電腦選擇一個位置
                pickBlock = randPickBlock();
            }
            PRINTL("玩家 %d 選擇在 %d 放置強盜", game->turn, pickBlock);
            if (checkRobberPos(pickBlock) == -1) {
                printGameInfo(USE_CARD);
                PRINTC(YELLOW, "%s\n", msg);
                PRINTC(YELLOW, "使用騎士卡失敗，請重新選擇");
                PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
                display();  // 按下 ENTER 繼續
                return -1;
            }
            placeRobberPos(pickBlock);

            printGameInfo(USE_CARD);
            printf("玩家%d 使用了騎士卡", game->turn);
            PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
            display();  // 按下 ENTER 繼續

            // 檢查是否有玩家可以搶奪
            if (checkRobbable(game->turn, pickBlock) == -1) break;
            FOREVER(Attempt) {
                int32_t pickPlayer;
                if (game->turn == PLAYER1) {
                    // 顯示輸入介面，並獲取玩家輸入的位置
                    printGameInfo(USE_CARD);
                    printf("該板塊上有其他玩家的村莊或城堡，可以搶奪玩家");
                    PRINTC(BLUE, ".........按下 [ENTER] 以關閉地圖，進行選擇\n");
                    display();  // 按下 ENTER 繼續
                    pickPlayer = readCMD("請選擇欲搶奪資源的玩家：", 2, 4, Attempt);
                } else {
                    // 讓電腦選擇一個位置
                    pickPlayer = randPickPlayer(pickBlock);
                }
                PRINTL("玩家 %d 選擇搶奪玩家 %d 的資源", game->turn, pickPlayer);
                if (checkRobAct(game->turn, pickBlock) == -1) continue;
                // 隨機搶奪一個資源
                randRobPlayer(pickPlayer);
                // 更新畫面
                printGameInfo(USE_CARD);
                printf("玩家%d 搶奪了玩家%d 的資源\n", game->turn, pickPlayer);
                PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
                display();  // 按下 ENTER 繼續
                break;
            }
            break;
        case ROAD_BUILDING:
            PRINTL("玩家 %d 使用道路建設卡", game->turn);
            for (int32_t idx = 0; idx < 2; idx++) {
                int32_t pickRoad;
                FOREVER(Attempt) {
                    if (game->turn == 1) {
                        // 顯示輸入介面，並獲取玩家輸入的位置
                        pickRoad = readPos("<使用建路卡>\n請輸入建設道路的位置：", T_ROAD, Attempt);
                    } else {
                        // TODO_T: 讓電腦選擇一個位置
                        pickRoad = randPickRoad();
                    }
                    PRINTL("玩家%d 修建了一條道路", game->turn);
                    if (checkRoad(game->turn, pickRoad) == -1) continue;
                    PRINTL("建設道路合法，進行建設");
                    placeRoad(game->turn, pickRoad);
                    break;
                }
            }
            // TODO_S: 更新畫面
            printGameInfo(USE_CARD);
            printf("玩家%d 使用了建路卡", game->turn);
            PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
            display();  // 按下 ENTER 繼續
            break;
        case YEAR_OF_PLENTY:
            PRINTL("玩家 %d 使用豐收卡", game->turn);
            for (int idx = 0; idx < 2; idx++) {
                FOREVER(Attempt) {
                    if (game->turn == 1) {
                        // 顯示輸入介面，並獲取玩家輸入的位置
                        printGameInfo(USE_CARD);
                        PRINTC(GREEN, "可以選擇的資源 >>> ");
                        printf(" 木頭[1] 磚塊[2] 羊毛[3] 小麥[4] 石頭[5]\n");
                        resourceType = readCMD("<使用豐收卡>\n請選擇獲得的資源(1~5)：", 1, 5, Attempt);
                    } else {
                        // 讓電腦選擇一個位置
                        resourceType = randPickResource();
                    }
                    if (resourceType == -1) {
                        setMsg("輸入錯誤 - 輸入的資源類型不存在，請重新輸入");
                        continue;
                    }
                    // 獲得資源
                    game->player[game->turn].resource[resourceType]++;
                    game->player[NONE].resource[resourceType]--;
                    break;
                }
            }
            // 更新畫面
            printGameInfo(USE_CARD);
            printf("玩家%d 使用豐收卡獲得了兩個資源", game->turn);
            PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
            display();  // 按下 ENTER 繼續
            break;
        case MONOPOLY:
            PRINTL("玩家 %d 使用獨佔卡", game->turn);
            FOREVER(Attempt) {
                if (game->turn == 1) {
                    // 顯示輸入介面，並獲取玩家輸入的位置
                    printGameInfo(USE_CARD);
                    PRINTC(GREEN, "可以選擇的資源 >>> ");
                    printf(" 木頭[1] 磚塊[2] 羊毛[3] 小麥[4] 石頭[5]\n");
                    resourceType = readCMD("<使用獨佔卡>\n請選擇獨佔的資源(1~5)：", 1, 5, Attempt);
                } else {
                    // TODO_T: 讓電腦選擇一個位置
                    resourceType = randPickResource();
                    DEV() {
                        printf("玩家 %d 請選擇獨佔的資源(0~4)：", game->turn);
                        scanf("%d", &resourceType);
                    }
                }
                if (resourceType == -1) {
                    setMsg("輸入錯誤 - 輸入的資源類型不存在，請重新輸入");
                    continue;
                }
                gainMonopolyResource(resourceType);
                break;
            }
            // 更新畫面
            printGameInfo(USE_CARD);
            printf("玩家%d 使用獨佔卡霸佔了一種資源", game->turn);
            PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
            display();  // 按下 ENTER 繼續
            break;
    }
    cardType->status = USED;
    return 0;
}

// 3.5 - STATE_BANK_TRADE
int32_t bankTrade() {
    int32_t pickResource[2];  // 要交易的資源：pickResource[0] 是要換出的資源，pickResource[1] 是要獲得的資源

    if (game->turn == 1) {
        // 顯示輸入介面，並獲取玩家輸入的位置
        readBankTrade(&pickResource[0], &pickResource[1], NONE);
    } else {
        // TODO_T: 讓電腦選擇一個位置
        pickResource[0] = 1;
        for (int i = 1; i < 6; i++) {
            if (game->player[game->turn].resource[i] > pickResource[0]) pickResource[0] = i;
        }
        pickResource[1] = randPickResource();
        // DEV() {
        //     printf("玩家 %d 請選擇換出的資源：", game->turn);
        //     scanf("%d", &pickResource[0]);
        //     printf("玩家 %d 請選擇獲得的資源：", game->turn);
        //     scanf("%d", &pickResource[1]);
        // }
    }
    PRINTL("玩家 %d 選擇以 %d 資源換取 %d 資源", game->turn, pickResource[0], pickResource[1]);
    if (checkBankTrade(game->turn, pickResource[0], pickResource[1]) == -1) return -1;
    PRINTL("交易合法，進行交易");
    // 銀行交易
    placeBankTrade(pickResource[0], pickResource[1]);
    // 更新畫面
    printGameInfo(NONE);
    printf("玩家%d 以 %d 資源換取了 %d 資源", game->turn, pickResource[0], pickResource[1]);
    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續
    return 0;
}

// 4 - STATE_ROBBER
int32_t robberAction() {
    int32_t pickBlock;
    int32_t lostResource[6];
    int32_t robPlayer;

    PRINTL("檢查有無玩家需要丟棄資源");
    for (int32_t playerIdx = 1; playerIdx <= 4; playerIdx++) {
        if (game->player[playerIdx].resource[ALL] < 8) continue;
        FOREVER(Attempt) {  // 直到玩家合法的丟出資源為止
            PRINTL("玩家 %d 需要丟棄資源卡 %d 張", playerIdx, game->player[playerIdx].resource[ALL] / 2);
            if (playerIdx == 1) {
                // 顯示輸入介面，並獲取玩家輸入的資源
                readDiscard(game->player[playerIdx].resource, lostResource, Attempt);
            } else {
                // 讓電腦選擇丟棄資源
                randLostResource(lostResource);
            }
            if (checkDiscard(playerIdx, lostResource) == -1) continue;
            break;
        }
        // 丟棄資源
        discardResource(playerIdx, lostResource);
        PRINTL("丟棄資源合法，丟棄資源");
        // 更新畫面
        printGameInfo(NONE);
        printf("玩家%d 丟棄了 %d 個資源", playerIdx, lostResource[ALL] / 2);
        PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
        display();  // 按下 ENTER 繼續
    }

    FOREVER(Attempt) {  // 直到玩家合法的選擇強盜位置為止
        if (game->turn == PLAYER1) {
            // 顯示輸入介面，並獲取玩家輸入的位置
            pickBlock = readPos("移動強盜到一個新板塊，請輸入 X Y 座標：", T_BLOCK, Attempt);
        } else {
            // 讓電腦選擇一個位置
            pickBlock = randPickBlock();
        }
        PRINTL("玩家 %d 選擇在 %d 放置強盜", game->turn, pickBlock);
        if (checkRobberPos(pickBlock) == -1) continue;
        break;
    }
    PRINTL("放置強盜合法，進行放置");
    placeRobberPos(pickBlock);
    // 更新畫面
    printGameInfo(NONE);
    printf("玩家%d 將強盜放置在 %d", game->turn, pickBlock);
    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續

    // 如果可以掠奪，選擇要掠奪的玩家
    if (checkRobbable(game->turn, pickBlock) == -1) return 0;
    PRINTL("可以掠奪，選擇要掠奪的玩家");
    FOREVER(Attempt) {  // 直到玩家合法的選擇掠奪玩家為止
        if (game->turn == PLAYER1) {
            // 顯示輸入介面，並獲取玩家輸入的玩家
            printGameInfo(NONE);
            robPlayer = readCMD("掠奪一個玩家，請輸入玩家編號(1-4)：", 2, 4, Attempt);
        } else {
            // 讓電腦選擇一個位置
            robPlayer = randRobPlayer(pickBlock);
        }
        PRINTL("玩家 %d 選擇掠奪玩家 %d", game->turn, robPlayer);
        if (checkRobAct(robPlayer, pickBlock) == -1) continue;
        break;
    }
    PRINTL("掠奪合法，進行掠奪");
    // 隨機掠奪一張資源
    randRobPlayer(robPlayer);
    // 更新畫面
    printGameInfo(NONE);
    printf("玩家%d 掠奪了 玩家%d 的資源", game->turn, robPlayer);
    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續
    game->state = BUILD;
    return 0;
}
