#include "catan.h"

pGame game;
bool cheat = false;
char msg[10000];
int roundCount;

int playGame(void) {
    init();
    // 遊戲開始
    game->state = SETTLE;
    printGameInfo(0);
    PRINTC(PURPLE, "卡坦島地圖加載完成，遊戲開始！\n");
    PRINTC(BLUE, ".........[按下 ENTER 繼續]");
    display();  // 按下 ENTER 繼續
    roundCount = 0;

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
                buildAction();
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
            userChoice = readCMD("你的選擇：", 0, 2, 0);
            if (userChoice == -1) setMsg("輸入錯誤 - 未知的指令，請重新輸入");
            if (userChoice == -1) continue;
            break;
        }
        switch (userChoice) {
            case 1:  // 開始新遊戲
                printf("     您選擇了一般模式，正在開啟卡坦島...\n");
                cheat = false;
                playGame();
                break;
            case 2:  // 載入遊戲
                printf("     您選擇了透視模式，正在加載透視眼鏡...\n");
                cheat = true;
                playGame();
                break;
            default:
                printf("     您選擇了離開遊戲，正在關閉卡坦島...\n");
                userChoice = 0;
                break;
        }
    }
    return 0;
}