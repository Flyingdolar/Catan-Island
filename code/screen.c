#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "catan.h"

#define WIDTH 900
#define HEIGHT 800
#define FRAMERATE 60
#define BGFILENAME "./resources/background.png"
#define FONTPATH "./resources/padmaa.ttf"
#define FONTSIZE 24
#define BLOCK_NUM 19

#define edgeWidth 25
#define edgeHeight 80
#define ROBBER_WIDTH 50
#define ROBBER_HEIGHT 50

SDL_Renderer *renderer;
SDL_Surface *bgImg;
SDL_Texture *bgTexture;
SDL_Texture *blockTexture[4][5];
SDL_Texture *villageTexture;
SDL_Texture *buttonTexture;
// SDL_Texture* cityTexture;
SDL_Rect buttonPositions[19][6];
SDL_Texture* robberTexture; 
SDL_Rect robberRect;
SDL_bool isDraggingRobber = SDL_FALSE;

TTF_Font* font;

pGame game;
SDL_Rect getEdgeRect(pBlock block, int blockIndex, int edgeIndex) {
    SDL_Rect rect;
    rect.x = buttonPositions[blockIndex][edgeIndex].x;
    rect.y = buttonPositions[blockIndex][edgeIndex].y;
    rect.w = edgeWidth;
    rect.h = edgeHeight;
    return rect;
}

int getRobberBlockIndex(int x, int y) {
    // how to get the robber block index? 先用矩形來做了
    for(int i = 0; i < BLOCK_NUM; i++) {
        if(x >= buttonPositions[i][0].x && x <= buttonPositions[i][2].x&&
           y >= buttonPositions[i][0].y && y <= buttonPositions[i][5].y) {
            return i;
        }
    }
    return -1;
}
void handleMouseClick_ROBBER(int x, int y) {

    // Render the robber image at the mouse location
    SDL_RenderCopy(renderer, robberTexture, NULL, &robberRect);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void handleMouseClick_BUILD(int x, int y) {
   
    // Check if the mouse click was inside one of the buttons
    for (size_t i = 0; i < BLOCK_NUM; i++)
    {
        for(size_t j = 0; i < 6; i++) {
            if(x >= buttonPositions[i][j].x && x <= buttonPositions[i][j].x + buttonPositions[i][j].w 
            && y >= buttonPositions[i][j].y && y <= buttonPositions[i][j].y + buttonPositions[i][j].h) {
                // Mouse click was inside this button, draw the village/city
                
                pList listNode = getNode(game->block, i);
                pBlock block = entry(listNode, sBlock);  // Get the block from the list element
                // TODO: Check if the block should be a village or city
                SDL_RenderCopy(renderer, villageTexture, NULL, &buttonPositions[i][j]);
                SDL_RenderPresent(renderer);

                //TODO: update the owner of the node
                //TODO: update the building of the player

                break;
            }
        }
    }

    pList element = game->block->NEXT;
    int index = 0;
    while (element != game->block) {
        pBlock block = entry(element, sBlock);
        // 遍历六边形的每一条边
        for (int i = 0; i < 6; i++) {
            // 判断鼠标的点击位置是否在这条边的矩形区域内
            SDL_Rect edgeRect = getEdgeRect(block, index, i);
            if (SDL_PointInRect(&(SDL_Point){x, y}, &edgeRect)) {
                // 1. 如果在，显示 road.png
                //SDL_Texture* roadTexture = IMG_LoadTexture(renderer, "road.png");
                //SDL_RenderCopy(renderer, roadTexture, NULL, &edgeRect);
                //SDL_DestroyTexture(roadTexture);

                // 2. 如果在，设置绘图颜色并填充这个矩形
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 设置为红色
                SDL_RenderFillRect(renderer, &edgeRect);
                SDL_RenderPresent(renderer); 
                //TODO: update the owner of the edge
                //TODO: update the roadlength of the player
                break;
            }
        }
        element = element->NEXT;
        index += 1;
    }
}

void drawNumber(int x, int y, int number){
    SDL_Color textColor;
    if (number == 6 || number == 8) {
        textColor.r = 255;
        textColor.g = 0;
        textColor.b = 0;
        textColor.a = 255; // Red
    } else {
        textColor.r = 0;
        textColor.g = 0;
        textColor.b = 0;
        textColor.a = 255; // Black
    }

    char numText[3];
    sprintf(numText, "%d", number); // Convert the number to a string

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, numText, textColor);
    if (textSurface == NULL) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Don't need the surface after texture is created

    //where to render the text.
    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture); 

}

void draw(SDL_Window *win) {
    //set background color
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    SDL_Rect src = {0, 0, bgImg->w, bgImg->h};
    SDL_RenderCopy(renderer, bgTexture, &src, NULL);

    //draw block
    pList element = game->block->NEXT; // Start from the first block
    int index = 0;
    while (element != game->block) { 
        pBlock block = entry(element, sBlock); // Get the block from the list element
        int xpos = 0;
        int ypos = 0;
        if(block->coord[0] <= 3){
            xpos = 248 - 68 * block->coord[1] + block->coord[0] * 136;
            ypos = 80 + block->coord[1] * 120;   
        }
        else{
            xpos = 180 + 68 * (block->coord[1] - 4) + block->coord[0] * 136;
            ypos = 440 + 120 * (block->coord[1] -4);
        }
        SDL_Rect dst = {xpos, ypos, 136, 160};
        SDL_RenderCopy(renderer, blockTexture[block->coord[0]][block->coord[1]], NULL, &dst);

        buttonPositions[index][0] = (SDL_Rect){xpos -15, ypos + 40 -15, 30, 30};
        buttonPositions[index][1] = (SDL_Rect){xpos + 68 -15, ypos -15, 30, 30};
        buttonPositions[index][2] = (SDL_Rect){xpos + 136 -15, ypos + 40 -15, 30, 30};
        buttonPositions[index][3] = (SDL_Rect){xpos + 136 -15, ypos + 120 -15, 30, 30};
        buttonPositions[index][4] = (SDL_Rect){xpos + 68 -15, ypos + 160 -15, 30, 30};
        buttonPositions[index][5] = (SDL_Rect){xpos -15, ypos + 120 -15, 30, 30};

        for(int i = 0; i < 6; i++) {
            SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonPositions[index][i]);
        }

        drawNumber(xpos+68, ypos+80, block->number);
        element = element->NEXT; // Move to the next block
        index += 1;
    }
    //update renderer
    SDL_RenderPresent(renderer);
}

void event_loop(SDL_Window *win) {
    while (1) {
        uint32_t begin = SDL_GetTicks();
        draw(win);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    //SDL_Log("Mouse move(x:%d,y:%d)", event.motion.x, event.motion.y);
                    if (game->state == ROBBER && isDraggingRobber) {
                        robberRect.x = event.motion.x;
                        robberRect.y = event.motion.y;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    //SDL_Log("Mouse down(x:%d,y:%d)", event.button.x, event.button.y);
                    if (game->state == ROBBER || game->state == SETTLE) {
                        isDraggingRobber = SDL_TRUE;
                        handleMouseClick_ROBBER(event.button.x, event.button.y);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    //SDL_Log("Mouse up(x:%d,y:%d)", event.button.x, event.button.y);
                    //handle mouse click event
                    if (game->state == BUILD || game->state == SETTLE) {
                        handleMouseClick_BUILD(event.button.x, event.button.y);
                    }
                    if (game->state == ROBBER) {
                        isDraggingRobber = SDL_FALSE;
                        //get the block index that the robber is on
                        int blockIndex = getRobberBlockIndex(event.button.x, event.button.y);
                    }
                    break;
                case SDL_QUIT:
                    return;
            }
        }
        long current = SDL_GetTicks();
        long cost = current - begin;
        long frame = 1000 / FRAMERATE;
        long delay = frame - cost;

        if (delay > 0) {
            SDL_Delay(delay);
        }
    }
}


int initScreen() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Can not init video, %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow(
            "Catan Island",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH, HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (win == NULL) {
        SDL_Log("Can not create window, %s", SDL_GetError());
        return 1;
    }


    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Can not create renderer, %s", SDL_GetError());
        return 1;
    }

    //load font
    font = TTF_OpenFont(FONTPATH, FONTSIZE);

    if (font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
    //load background image
    bgImg = IMG_Load(BGFILENAME);
    if (bgImg == NULL)
    {
        SDL_Log("Can not load bg image, %s", SDL_GetError());
        return 1;
    }
    //create bg image texture
    bgTexture = SDL_CreateTextureFromSurface(renderer, bgImg);
    if (bgTexture == NULL)
    {
        SDL_Log("Can not create bg texture, %s", SDL_GetError());
        return 1;
    }
    //load robber image
    robberTexture = IMG_LoadTexture(renderer, "./resources/robber.png");

    //define block image file name array
    char blockFileName[BLOCK_NUM][45] = {"./resources/dessert.png", "./resources/mountain.png","./resources/forest.png","./resources/wheat_field.png","./resources/hill.png","./resources/grass.png"};
    //create block image surface array
    SDL_Surface *blockImg[4][5];

    //load block image and create block image texture array
    pList element = game->block->NEXT; // Start from the first block
    while (element != game->block) { 
        pBlock block = entry(element, sBlock); // Get the block from the list element
        blockImg[block->coord[0]][block->coord[1]] = IMG_Load(blockFileName[block->resource]);
        if (blockImg[block->coord[0]][block->coord[1]] == NULL)
        {
            SDL_Log("Can not load block image, %s", SDL_GetError());
            return 1;
        }
        blockTexture[block->coord[0]][block->coord[1]] = IMG_LoadTexture(renderer, blockFileName[block->resource]);
        element = element->NEXT; // Move to the next block
    }
    
    buttonTexture = IMG_LoadTexture(renderer, "./resources/button.png");
    villageTexture = IMG_LoadTexture(renderer, "./resources/village.png");
    //cityTexture = IMG_LoadTexture(renderer, "city.png");
    
    event_loop(win);
    SDL_FreeSurface(bgImg);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(win);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}