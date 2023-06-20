#include "catan.h"

pGame game;
bool cheat = false;
char msg[10000];
int roundCount = 0;

void test() {
    game->state = SETTLE;

    game->turn = 1;
    placeNode(1, 0);
    gainSettleResource(1, 0);
    gainSettleResource(1, 3);
    gainSettleResource(1, 6);
    gainSettleResource(1, 9);
    gainSettleResource(1, 12);
    placeNode(1, 2);
    gainSettleResource(1, 10);
    gainSettleResource(1, 14);
    gainSettleResource(1, 16);
    gainSettleResource(1, 18);
    gainSettleResource(1, 20);
    placeNode(2, 4);
    game->turn = 2;
    gainSettleResource(2, 4);
    placeNode(2, 6);
    gainSettleResource(2, 6);
    game->turn = 3;
    placeNode(3, 16);
    gainSettleResource(3, 16);
    placeNode(3, 18);
    gainSettleResource(3, 18);
    game->turn = 4;
    placeNode(4, 20);
    gainSettleResource(4, 20);
    placeNode(4, 22);
    gainSettleResource(4, 22);

    placeRoad(1, 0);
    placeRoad(1, 1);

    game->turn = 1;
    game->armyKing.owner = 1;
    game->roadKing.owner = 1;
    game->state = DICE;
    return;
}

int playGame(int testMode) {
    init();
    // 遊戲開始
    game->state = SETTLE;
    PRINTC(PURPLE, "卡坦島地圖加載完成，遊戲開始！\n");
    if (testMode == 1) {
        PRINTC(PURPLE, "測試模式啟動，正在加載相關資源...\n");
        test();
        PRINTC(GREEN, "測試模式加載完成，遊戲開始！\n");
        game->state = DICE;
    }
    printGameInfo(0);
    PRINTC(BLUE, ".........[按下 ENTER 繼續]\n");
    display();  // 按下 ENTER 繼續

    // 直到分出勝負前，遊戲都不會結束
    while (1) {
        if (game->state == END) break;  // 有玩家達成勝利條件，遊戲結束
        switch (game->state) {
            case START:
                PRINTD("遊戲尚未初始化，發生錯誤");
                return -1;
            case SETTLE:  // 首輪回合，放置兩個村莊
                PRINTL("遊戲開始，請依序放置兩個村莊");
                settleAction();
                break;
            case DICE:  // 每回合的骰子行動
                PRINTL("輪到 %d 骰子行動", game->turn);
                diceAction();
                break;
            case BUILD:  // 每回合的建設行動
                PRINTL("輪到 %d 建設行動", game->turn);
                if (buildAction() == 1) return 0;
                break;
            case ROBBER:  // 每回合的強盜行動
                PRINTL("輪到 %d 強盜行動", game->turn);
                robberAction();
                break;
            default:
                break;
        }
    }
    // TODO: 顯示遊戲最終由誰獲勝
    //
    return 0;
}

int main(/*int argc, char *argv[]*/) {
    int userChoice = 1;

    while (userChoice != 0) {
        while (1) {
            CLEAR();
            printf("------歡迎遊玩卡坦島，請選擇遊戲模式------\n");
            printf("               一般模式[1]\n");
            printf("               透視模式[2]\n");
            printf("               離開遊戲[0]\n");
            printf("-------------------------------------------\n");
            userChoice = readCMD("你的選擇：", 0, 3, 0);
            if (userChoice == -1) setMsg("輸入錯誤 - 未知的指令，請重新輸入\n");
            if (userChoice == -1) continue;
            break;
        }
        switch (userChoice) {
            case 1:  // 開始新遊戲
                printf("     您選擇了一般模式，正在開啟卡坦島...\n");
                cheat = false;
                playGame(0);
                break;
            case 2:  // 載入遊戲
                printf("     您選擇了透視模式，正在加載透視眼鏡...\n");
                cheat = true;
                playGame(0);
                break;
            case 3:  // 測試模式
                printf("     您選擇了測試模式，正在開啟卡坦島...\n");
                cheat = true;
                playGame(1);
                break;
            default:
                printf("     您選擇了離開遊戲，正在關閉卡坦島...\n");
                userChoice = 0;
                break;
        }
    }
    return 0;
}
