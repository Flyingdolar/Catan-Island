#include "catan.h"

int32_t readCMD(char *string, int32_t lower, int32_t upper, int32_t tryTime) {
    char cmd[1000];
    int number = 0;
    if (string != NULL) printf("\n");
    if (tryTime > 0) PRINTC(YELLOW, "%s", msg);
    if (string != NULL) printf("%s", string);
    fgets(cmd, 1000, stdin);
    // if cmd is not number, return -1
    for (size_t i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == '\n') {
            if (i == 0) return -1;
            if (number < lower || number > upper) return -1;
            return number;
        }
        if (cmd[i] >= '0' && cmd[i] <= '9') {
            number = number * 10 + cmd[i] - '0';
            continue;
        }
        return -1;
    }
    return number;
}

int32_t readCard(char *string, int32_t tryTime) {
    int32_t infoMode = 0;

    if (tryTime < 0) infoMode -= tryTime;
    printGameInfo(infoMode);
    if (tryTime > 0) PRINTC(YELLOW, "%s", msg);
    PRINTC(GREEN, "可使用卡片一覽 >>>");
    forList(game->player[game->turn].devcard, cardList) {
        pDevcard card = entry(cardList, sDevcard);
        switch (card->type) {
            case KNIGHT:
                if (card->status == AVAILABLE)
                    printf(" 騎士卡[%zu]", cardList->index);
                break;
            case ROAD_BUILDING:
                if (card->status == AVAILABLE)
                    printf(" 建路卡[%zu]", cardList->index);
                break;
            case MONOPOLY:
                if (card->status == AVAILABLE)
                    printf(" 壟斷卡[%zu]", cardList->index);
                break;
            case YEAR_OF_PLENTY:
                if (card->status == AVAILABLE)
                    printf(" 豐收卡[%zu]", cardList->index);
                break;
            default:
                break;
        }
    }
    printf("\n");
    if (string != NULL) printf("\n%s", string);
    return readCMD(NO_ARG);
}

int32_t readDiscard(uint8_t *hold, int32_t array[6], int32_t tryTime) {
    int32_t wood = -2, brick = -2, sheep = -2, wheat = -2, ore = -2;

    while (1) {
        printGameInfo(0);
        if (tryTime > 0) PRINTC(YELLOW, "%s", msg);
        if (wood == -1 || brick == -1 || sheep == -1 || wheat == -1 || ore == -1)
            PRINTC(YELLOW, "  輸入格式錯誤，請重新輸入");
        printf("\n 需要棄掉資源 >>>  木頭  磚塊  羊毛  小麥  石頭\n");
        printf("    輸入格式 >>>  ");
        if (wood >= 0) printf("%d", wood);
        if (wood < 0) {
            wood = readCMD(NO_ARG);
            continue;
        }
        printf("    ");
        if (brick >= 0) printf("%d", brick);
        if (brick < 0) {
            brick = readCMD(NO_ARG);
            continue;
        }
        printf("    ");
        if (sheep >= 0) printf("%d", sheep);
        if (sheep < 0) {
            sheep = readCMD(NO_ARG);
            continue;
        }
        printf("    ");
        if (wheat >= 0) printf("%d", wheat);
        if (wheat < 0) {
            wheat = readCMD(NO_ARG);
            continue;
        }
        printf("    ");
        if (ore >= 0) printf("%d", ore);
        if (ore < 0) {
            ore = readCMD(NO_ARG);
            continue;
        }
        if (wood > hold[1] || brick > hold[2] || sheep > hold[3] || wheat > hold[4] || ore > hold[5]) {
            wood = -2, brick = -2, sheep = -2, wheat = -2, ore = -2, tryTime++;
            setMsg("  輸入資源超過擁有資源，請重新輸入");
            continue;
        }
        break;
    }
    array[1] = wood, array[2] = brick, array[3] = sheep, array[4] = wheat, array[5] = ore;
    array[0] = wood + brick + sheep + wheat + ore;
    return 0;
}

int32_t readPos(char *string, int32_t type, int32_t tryTime) {
    int32_t posX = -2, posY = -2;
    int32_t infoMode = 0;

    if (tryTime < 0) infoMode -= tryTime;
    while (posX < 0 || posY < 0) {
        printGameInfo(infoMode);
        if (tryTime > 0) PRINTC(YELLOW, "%s", msg);
        if (posX == -1 || posY == -1)
            PRINTC(YELLOW, "  輸入格式錯誤，請重新輸入");
        printf("\n%s  x= ", string);
        if (posX >= 0) printf("%d", posX);
        if (posX < 0) {
            posX = readCMD(NO_ARG);
            continue;
        }
        printf("  y= ");
        posY = readCMD(NO_ARG);
    }

    switch (type) {
        case T_BLOCK:
            forList(game->block, blockList) {
                pBlock block = entry(blockList, sBlock);
                if (block->coord[0] == posY && block->coord[1] == posX) return blockList->index;
            }
            return -1;
        case T_BUILD:
            forList(game->node, nodeList) {
                pNode build = entry(nodeList, sNode);
                if (build->coord[0] == posY && build->coord[1] == posX) return nodeList->index;
            }
            return -1;
        case T_ROAD:
            forList(game->road, roadList) {
                pRoad road = entry(roadList, sRoad);
                if (road->coord[0] == posY && road->coord[1] == posX) return roadList->index;
            }
            return -1;
    }
    return -1;
}

int32_t readBankTrade(int32_t *giveRes, int32_t *takeRes, int32_t tryTime) {
    int32_t give = -2, take = -2;
    while (1) {
        printGameInfo(TRADE_N);
        if (tryTime > 0) PRINTC(YELLOW, "%s", msg);
        if (give == -1 || take == -1)
            PRINTC(YELLOW, "  輸入格式錯誤，請重新輸入");
        printf("\n 請選擇交換資源種類 >>> 木頭[1] 磚塊[2]  羊毛[3]  小麥[4]  石頭[5]\n");
        printf("決定換出資源: ");
        if (give >= 0) printf("%d", give);
        if (give < 0) {
            give = readCMD(NO_ARG);
            continue;
        }
        printf("  決定換入資源: ");
        if (take >= 0) printf("%d", take);
        if (take < 0) {
            take = readCMD(NO_ARG);
            continue;
        }
        if (give == take) {
            give = -2, take = -2, tryTime++;
            setMsg("  輸入資源種類重複，請重新輸入");
            continue;
        }
        break;
    }
    *giveRes = give, *takeRes = take;
    return 0;
}

int32_t printGameInfo(int32_t buildOption) {
    CLEAR();
    //printGraph();
    printf("玩家1[你]  分數：%2d", game->player[1].score);
    if (game->roadKing.owner == 1) printf("  { 道路王者 }  ");
    if (game->armyKing.owner == 1) printf("  { 軍隊王者 }  ");
    printf("\n-----------------------------------------------------------------------\n");
    printf("||  持有資源   || 木頭：%2d | 磚塊：%2d | 羊毛：%2d | 小麥：%2d | 石頭：%2d |\n",
           game->player[1].resource[WOOD], game->player[1].resource[BRICK],
           game->player[1].resource[SHEEP], game->player[1].resource[WHEAT],
           game->player[1].resource[ORE]);
    printf("||  持有點數   || 道路長度：%2d | 軍隊數量：%2d | 城鎮：%2d | 城市：%2d |\n",
           game->player[1].roadlength, game->player[1].armySize,
           game->player[1].building[VILLAGE], game->player[1].building[CITY]);
    printf("||  持有發展卡 |");
    forList(game->player[1].devcard, cardList) {
        pDevcard card = entry(cardList, sDevcard);
        switch (card->type) {
            case VICTORY_POINT:
                printf("| 勝利點數卡");
                break;
            case KNIGHT:
                if (card->status == AVAILABLE)
                    printf("| 騎士卡 ");
                if (card->status == UNUSED)
                    printf("| 騎士卡(準備中) ");
                break;
            case ROAD_BUILDING:
                if (card->status == AVAILABLE)
                    printf("| 建路卡 ");
                if (card->status == UNUSED)
                    printf("| 建路卡(準備中) ");
                break;
            case YEAR_OF_PLENTY:
                if (card->status == AVAILABLE)
                    printf("| 豐收卡 ");
                if (card->status == UNUSED)
                    printf("| 豐收卡(準備中) ");
                break;
            case MONOPOLY:
                if (card->status == AVAILABLE)
                    printf("| 壟斷卡 ");
                if (card->status == UNUSED)
                    printf("| 壟斷卡(準備中) ");
                break;
            default:
                break;
        }
    }
    printf("|\n");

    for (size_t idx = 2; idx < 5; idx++) {
        printf("\n");
        printf("玩家%zu[電腦]  分數：%2d", idx, game->player[idx].score);
        if (game->roadKing.owner == idx) printf("  { 道路王者 }  ");
        if (game->armyKing.owner == idx) printf("  { 軍隊王者 }  ");
        printf("\n-----------------------------------------------------------------------\n");
        printf("||  持有資源   || %2d 張 |\n", game->player[idx].resource[ALL]);
        printf("||  持有點數   || 道路長度：%2d | 軍隊數量：%2d | 城鎮：%2d | 城市：%2d |\n",
               game->player[idx].roadlength, game->player[idx].armySize,
               game->player[idx].building[VILLAGE], game->player[idx].building[CITY]);
        int32_t cardNum = 0;
        forList(game->player[idx].devcard, cardList) {
            pDevcard card = entry(cardList, sDevcard);
            if (card->type != VICTORY_POINT && card->status == USED) continue;
            cardNum++;
        }
        printf("||  持有發展卡 || %2d 張 |\n", cardNum);
    }
    printf("\n\n");
    printf("++++++++++++++ 第 %d 輪 - 現在輪到 玩家%d 的回合 ++++++++++++++\n", roundCount, game->turn);
    if (game->state == SETTLE) printf("[準備階段]\n");
    if (game->state == DICE) printf("[擲骰階段]\n");
    if (game->state == BUILD) {
        printf("[建設階段]");
        if (buildOption == BUILD_ROAD) printf(" - 建造道路");
        if (buildOption == BUILD_NODE) printf(" - 建造村莊");
        if (buildOption == BUY_CARD) printf(" - 購買卡片");
        if (buildOption == USE_CARD) printf(" - 使用卡片");
        if (buildOption == TRADE_N) printf(" - 銀行交易");
        printf("\n");
    }
    return 0;
}
