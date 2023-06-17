#include "catan.h"

pGame game;

int playGame(int isNewGame) {
    if (isNewGame > 0) init();
    if (game == NULL) {
        PRINTD("遊戲初始化失敗");
        return -1;
    }

    // 直到分出勝負前，遊戲都不會結束
    while (1) {
        if (game->state == END) break;  // 有玩家達成勝利條件，遊戲結束
        switch (game->state) {
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
                buildAction();
                break;
            case ROBBER:  // 每回合的強盜行動
                PRINTL("輪到 %d 強盜行動", game->turn);
                robberAction();
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    int userChoice = 1;

    // TODO: 開啟 SDL 視窗，並初始化
    //
    // TODO: 開啟 Menu 視窗，顯示選項：開始遊戲、載入遊戲、離開遊戲
    //
    while (userChoice != 0) {
        // TODO: 從圖形化介面視窗獲取使用者選擇
        DEV() {
            printf("請選擇遊戲模式：\n");
            scanf("%d", &userChoice);
        }
        switch (userChoice) {
            case 1:  // 開始新遊戲
                playGame(1);
                break;
            case 2:  // 載入遊戲
                playGame(0);
            default:
                break;
        }
    }
    return 0;
}
