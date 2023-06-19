#include "catan.h"

void printGraph() {
    printf("Print Blocks:(點數(資源))\n");
    forList(game->block, listNode) {
        pBlock block = entry(listNode, sBlock);
        if (listNode->index == 0) printf("     ");
        if (listNode->index == 3) printf("  ");
        if (listNode->index == 12) printf("  ");
        if (listNode->index == 16) printf("     ");
        printf(" %d(%d)", block->number, block->resource);
        if (listNode->index == 2) printf("\n");
        if (listNode->index == 6) printf("\n");
        if (listNode->index == 11) printf("\n");
        if (listNode->index == 15) printf("\n");
        if (listNode->index == 18) printf("\n");
    }
    printf("Print Nodes:(持有者(建築))\n");
    forList(game->node, listNode) {
        if (listNode->index == 0) printf("          ");
        if (listNode->index == 7) printf("     ");
        if (listNode->index == 38) printf("     ");
        if (listNode->index == 47) printf("          ");
        pNode node = entry(listNode, sNode);
        printf(" %d(%d)", node->owner, node->building);
        if (listNode->index == 6) printf("\n");
        if (listNode->index == 15) printf("\n");
        if (listNode->index == 26) printf("\n");
        if (listNode->index == 37) printf("\n");
        if (listNode->index == 46) printf("\n");
    }
    printf("\nPrint Roads:\n");
    forList(game->road, listNode) {
        if (listNode->index == 0) printf("      ");
        if (listNode->index == 6) printf("     ");
        if (listNode->index == 10) printf("    ");
        if (listNode->index == 18) printf("   ");
        if (listNode->index == 23) printf("  ");
        if (listNode->index == 33) printf(" ");
        if (listNode->index == 39) printf("  ");
        if (listNode->index == 49) printf("   ");
        if (listNode->index == 54) printf("    ");
        if (listNode->index == 62) printf("     ");
        if (listNode->index == 66) printf("      ");
        pRoad road = entry(listNode, sRoad);
        if (road->coord[0] % 2 == 1 && road->coord[1] != 0) printf("  ");
        printf(" %d", road->owner);
        if (listNode->index == 5) printf("\n");
        if (listNode->index == 9) printf("\n");
        if (listNode->index == 17) printf("\n");
        if (listNode->index == 22) printf("\n");
        if (listNode->index == 32) printf("\n");
        if (listNode->index == 38) printf("\n");
        if (listNode->index == 48) printf("\n");
        if (listNode->index == 53) printf("\n");
        if (listNode->index == 61) printf("\n");
        if (listNode->index == 65) printf("\n");
        if (listNode->index == 71) printf("\n");
    }
    printf("\n\n");

    // printf("Print Players:\n");
    // for (int idx = 0; idx < 4; idx++) {
    //     pPlayer player = &game->player[idx];
    //     printf("Player %d: %d pt\n", idx, player->score);
    //     printf("--Resource(木/磚/羊/稻/石): %d %d %d %d %d\n", player->resource[0], player->resource[1], player->resource[2], player->resource[3], player->resource[4]);
    //     printf("--DevCard:");
    //     forList(player->devcard, listNode) {
    //         pDevcard devCard = entry(listNode, sDevcard);
    //         printf(" %d", devCard->type);
    //     }
    //     printf("\n");
    //     printf("--Building(路/村/市):");
    //     for (int idx = 0; idx < 3; idx++) {
    //         printf(" %d", player->building[idx]);
    //     }
    //     printf("\n");
    //     printf("--Harbor(3/木/磚/羊/稻/石):");
    //     for (int idx = 0; idx < 6; idx++) {
    //         printf(" %d", player->harbor[idx]);
    //     }
    //     printf("\n");
    //     printf("--King(Road/Army):");
    //     printf(" %d", player->roadlength);
    //     printf(" %d\n\n", player->armySize);
    // }
}
