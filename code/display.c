#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "catan.h"

#define WIDTH 900
#define HEIGHT 800
#define FRAMERATE 60

#define BGFILENAME "./resources/background.png"
#define FONTPATH "./resources/Ubuntu-Bold.ttf"
#define ROBBERPATH "./resources/robber.png"
#define NODEPATH "./resources/button.png"

#define FONTSIZE 50
#define BLOCK_NUM 19

#define ROAD_WIDTH 20
#define ROAD_HEIGHT 60

#define ROBBER_WIDTH 50
#define ROBBER_HEIGHT 100

#define NODE_WIDTH 70
#define NODE_HEIGHT 70

/*Usage:
if (need to display something ) {
    Display* display = create_display();
    if (display) {
        // 更新 display
        update_display(display);
        // 等待一段時間或者直到某些條件滿足
    }
}
*/

Display* create_display() {
    //初始化Display
    Display* display = malloc(sizeof(Display));
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Can not init video, %s", SDL_GetError());
        free(display);
        return NULL;
    }
    //初始化字體
    if (TTF_Init()) {
        SDL_Log("Can not init TTF, %s", SDL_GetError());
        free(display);
        return NULL;
    }

    //創建窗口

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screenWidth = dm.w;
    // int screenHeight = dm.h;
    int windowX = screenWidth - WIDTH - 80;  // 计算窗口的x位置

    display->window = SDL_CreateWindow(
        "Catan Island",
        windowX,  // 将窗口放置在屏幕右侧
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);

    if (display->window == NULL) {
        SDL_Log("Can not create window, %s", SDL_GetError());
        free(display);
        return NULL;
    }

    //創建renderer
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    if (display->renderer == NULL) {
        SDL_Log("Can not create renderer, %s", SDL_GetError());
        free(display);
        return NULL;
    }

    //創建字體
    display->font = TTF_OpenFont(FONTPATH, FONTSIZE);
    if (display->font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        free(display);
        return NULL;
    }

    //創建背景
    SDL_Surface* bgImg = IMG_Load(BGFILENAME);
    if (bgImg == NULL) {
        SDL_Log("Can not load bg image, %s", SDL_GetError());
        free(display);
        return NULL;
    }

    display->bgTexture = SDL_CreateTextureFromSurface(display->renderer, bgImg);
    if (display->bgTexture == NULL) {
        SDL_Log("Can not create bg texture, %s", SDL_GetError());
        free(display);
        return NULL;
    }

    //創建強盜
    display->robberTexture = IMG_LoadTexture(display->renderer, ROBBERPATH);
    if (display->robberTexture == NULL) {
        SDL_Log("Can not load robber image, %s", SDL_GetError());
        free(display);
        return NULL;
    }
    //創建按鈕
    display->nodeTexture = IMG_LoadTexture(display->renderer, NODEPATH);
    if (display->nodeTexture == NULL) {
        SDL_Log("Can not load node image, %s", SDL_GetError());
        free(display);
        return NULL;
    }
    //創建village & city
    char villageFileName[4][100] = {"./resources/village1.png",
                                    "./resources/village2.png",
                                    "./resources/village3.png",
                                    "./resources/village4.png"};

    char cityFileName[4][100] = {"./resources/city1.png",
                                 "./resources/city2.png",
                                 "./resources/city3.png",
                                 "./resources/city4.png"};

    for (int i = 0; i < 4; i++) {
        display->villageTexture[i] = IMG_LoadTexture(display->renderer, villageFileName[i]);
        display->cityTexture[i] = IMG_LoadTexture(display->renderer, cityFileName[i]);
        if (display->villageTexture[i] == NULL || display->cityTexture[i] == NULL) {
            SDL_Log("Can not load village or city image, %s", SDL_GetError());
            free(display);
            return NULL;
        }
    }

    //創建block
    // define block image file name array
    char blockFileName[BLOCK_NUM][105] = {"./resources/dessert.png",
                                          "./resources/mountain.png",
                                          "./resources/forest.png",
                                          "./resources/wheat_field.png",
                                          "./resources/hill.png",
                                          "./resources/grass.png"};
    // create block image surface array
    SDL_Surface* blockImg[4][5];

    // load block image and create block image texture array
    pList element = game->block->NEXT;  // Start from the first block
    while (element != game->block) {
        pBlock block = entry(element, sBlock);  // Get the block from the list element
        blockImg[block->coord[0]][block->coord[1]] = IMG_Load(blockFileName[block->resource]);
        if (blockImg[block->coord[0]][block->coord[1]] == NULL) {
            SDL_Log("Can not load block image, %s", SDL_GetError());
            return NULL;
        }

        display->blockTexture[block->coord[0]][block->coord[1]] = IMG_LoadTexture(display->renderer, blockFileName[block->resource]);
        element = element->NEXT;  // Move to the next block
    }
    // block positions init
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 6; j++) {
            display->blockPositons[i][j] = (SDL_Rect){0, 0, 0, 0};
        }
    }
    // node positions init
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 11; j++) {
            display->nodePositions[i][j] = (SDL_Rect){0, 0, 0, 0};
        }
    }

    // 返回 display 結構
    return display;
}

void renderBlocksAndNumber(Display* display) {
    pList element = game->block->NEXT;  // Start from the first block
    // int index = 0;
    while (element != game->block) {
        int xpos = 0;
        int ypos = 0;
        pBlock block = entry(element, sBlock);  // Get the block from the list element
        if (block->coord[0] <= 2) {
            xpos = 248 - 68 * block->coord[0] + block->coord[1] * 136;
            ypos = 80 + block->coord[0] * 120;
        } else {
            xpos = 180 + 68 * (block->coord[0] - 3) + block->coord[1] * 136;
            ypos = 440 + 120 * (block->coord[0] - 3);
        }
        display->blockPositons[block->coord[0]][block->coord[1]] = (SDL_Rect){xpos, ypos, 136, 160};
        // printf("index: %d, xpos: %d, ypos: %d\n", index, xpos, ypos);
        SDL_RenderCopy(display->renderer, display->blockTexture[block->coord[0]][block->coord[1]], NULL, &display->blockPositons[block->coord[0]][block->coord[1]]);

        // number
        SDL_Color textColor;
        if (block->number == 6 || block->number == 8) {
            // red
            textColor = (SDL_Color){250, 0, 0, 250};
        } else {
            // white
            textColor = (SDL_Color){244, 234, 219, 250};
        }
        char number[3];
        sprintf(number, "%d", block->number);
        SDL_Surface* surface = TTF_RenderText_Solid(display->font, number, textColor);
        SDL_Rect textRect = {xpos + 68 - 20, ypos + 80 - 22, surface->w, surface->h};
        SDL_Texture* texture = SDL_CreateTextureFromSurface(display->renderer, surface);
        SDL_RenderCopy(display->renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        element = element->NEXT;  // Move to the next block
        // index++;
    }
}
void renderNodesAndVillage(Display* display) {
    pList element = game->node->NEXT;  // Start from the first node
    // int index = 0;
    while (element != game->node) {
        int xpos = 0;
        int ypos = 0;
        pNode node = entry(element, sNode);  // Get the node from the list element
        if (node->coord[0] <= 2) {
            xpos = 248 - 68 * node->coord[0] + node->coord[1] * 68 - NODE_WIDTH / 2;
            ypos = 120 + node->coord[0] * 120 - 40 * (node->coord[1] % 2) - NODE_WIDTH / 2;
        } else {
            xpos = 112 + 68 * (node->coord[0] - 3) + 68 * node->coord[1] - NODE_WIDTH / 2;
            ypos = 440 + 120 * (node->coord[0] - 3) + 40 * (node->coord[1] % 2) - NODE_WIDTH / 2;
        }
        display->nodePositions[node->coord[0]][node->coord[1]] = (SDL_Rect){xpos, ypos, NODE_WIDTH, NODE_HEIGHT};
        // printf("index: %d, xpos: %d, ypos: %d\n", index, xpos, ypos);
        if (node->building == 0) {
            SDL_RenderCopy(display->renderer, display->nodeTexture, NULL, &display->nodePositions[node->coord[0]][node->coord[1]]);
        } else if (node->owner != 0) {
            if (node->building == VILLAGE) {
                SDL_RenderCopy(display->renderer, display->villageTexture[node->owner - 1], NULL, &display->nodePositions[node->coord[0]][node->coord[1]]);
            } else if (node->building == CITY) {
                SDL_RenderCopy(display->renderer, display->cityTexture[node->owner - 1], NULL, &display->nodePositions[node->coord[0]][node->coord[1]]);
            }
        }
        element = element->NEXT;  // Move to the next node
        // index++;
    }
}

void renderRoads(Display* display) {
    // pList element = game->road->NEXT;  // Start from the first road
    // int index = 0;
    forList(game->road, element) {
        int xpos = 0;
        int ypos = 0;
        pRoad road = entry(element, sRoad);  // Get the road from the list element
        if ((road->coord[0] % 2) == 1) {     // vertical
            if (road->coord[0] <= 5) {
                xpos = 316 - 68 * (road->coord[0] + 1)/2 + road->coord[1] * 136 - ROAD_WIDTH/2;
                ypos = 80 + (road->coord[0]/2) * 120 + ROAD_HEIGHT/2;
                } else {
                xpos = 180 + 68 * ((road->coord[0] - 7) / 2) + road->coord[1] * 136 - ROAD_WIDTH / 2;
                ypos = 440 + 120 * ((road->coord[0] - 7) / 2) + ROAD_HEIGHT / 2;
            }
        } else {  //斜
            ypos = 100 + 120 * (road->coord[0] / 2) - ROAD_HEIGHT / 2;
            if (road->coord[0] <= 5) {
                xpos = 282 - 68 * (road->coord[0] / 2) + road->coord[1] * 68 - ROAD_WIDTH / 2;
            } else {
                xpos = 146 + 68 * ((road->coord[0] - 6) / 2) + road->coord[1] * 68 - ROAD_WIDTH / 2;
            }
        }
        if (road->owner == 0) {
            continue;
        }

        int r, g, b;
        switch (road->owner) {
            case 1:  // red
                r = 255;
                g = 0;
                b = 0;
                break;
            case 2:  // yellow
                r = 255;
                g = 255;
                b = 0;
                break;
            case 3:  // blue
                r = 0;
                g = 0;
                b = 255;
                break;
            case 4:  // green
                r = 0;
                g = 255;
                b = 0;
                break;
            default:
                break;
        }
        int rotate_angle = 0;
        if ((road->coord[0] <= 5 && road->coord[1] % 2 == 0) || (road->coord[0] > 5 && road->coord[1] % 2 == 1)) {
            rotate_angle = 60;
        } else if ((road->coord[0] <= 5 && road->coord[1] % 2 == 1) || (road->coord[0] > 5 && road->coord[1] % 2 == 0)) {
            rotate_angle = 120;
        }

        SDL_Rect roadRect = (SDL_Rect){xpos, ypos, ROAD_WIDTH, ROAD_HEIGHT};

        if ((road->coord[0] % 2) == 1) {  // vertical draw rect
            SDL_SetRenderDrawColor(display->renderer, r, g, b, 255);
            SDL_RenderFillRect(display->renderer, &roadRect);
        } else {
            SDL_Surface* surface = SDL_CreateRGBSurface(0, ROAD_WIDTH, ROAD_HEIGHT, 32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
            SDL_Texture* texture = SDL_CreateTextureFromSurface(display->renderer, surface);
            SDL_FreeSurface(surface);
            SDL_RenderCopyEx(display->renderer, texture, NULL, &roadRect, rotate_angle, NULL, SDL_FLIP_NONE);
        }
        element = element->NEXT;  // Move to the next road
        // index++;
    }
}

void update_display(Display* display) {
    // set background color
    SDL_SetRenderDrawColor(display->renderer, 1, 148, 202, 255);

    // 更新和繪製 display
    SDL_RenderClear(display->renderer);
    SDL_RenderCopy(display->renderer, display->bgTexture, NULL, NULL);
    renderBlocksAndNumber(display);
    renderNodesAndVillage(display);
    renderRoads(display);
    int robberPosX = display->blockPositons[game->robber->coord[0]][game->robber->coord[1]].x + 70;
    int robberPosY = display->blockPositons[game->robber->coord[0]][game->robber->coord[1]].y + 50;
    SDL_Rect robberRect = (SDL_Rect){robberPosX, robberPosY, ROBBER_WIDTH, ROBBER_HEIGHT};
    SDL_RenderCopy(display->renderer, display->robberTexture, NULL, &(robberRect));

    SDL_RenderPresent(display->renderer);
}

void destroy_display(Display* display) {
    // 銷毀窗口和 renderer，關閉 SDL
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if ((i == 0 || i == 4) && j > 2) continue;
            if ((i == 1 || i == 3) && j > 3) continue;
            SDL_DestroyTexture(display->blockTexture[i][j]);
        }
    }
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(display->cityTexture[i]);
        SDL_DestroyTexture(display->villageTexture[i]);
    }
    SDL_DestroyTexture(display->bgTexture);
    SDL_DestroyTexture(display->robberTexture);
    SDL_DestroyTexture(display->nodeTexture);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    TTF_CloseFont(display->font);
    TTF_Quit();
    SDL_Quit();
    free(display);
}

int event_loop(Display* display) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            destroy_display(display);
            SDL_Quit();
            return -1;  //點擊關閉視窗
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN) {
                destroy_display(display);
                SDL_Quit();
                return -1;  // 代表用户按下了 Enter 键
            }
        }
    }
    return 0;
}

int display() {
    Display* display = create_display();
    if (display) {
        while (1) {
            update_display(display);
            if (event_loop(display) == -1) {
                return -1;
            }
            SDL_Delay(1000 / FRAMERATE);
        }
    }
    return 0;
}
