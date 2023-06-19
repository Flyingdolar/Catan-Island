/*
typedef struct _sGame_{ // 遊戲
    eState  state    ;  // --遊戲狀態 0~5
    uint8_t turn     ;  // --輪到誰 1~4
    uint8_t dice[2]  ;  // --骰子點數 2個1~6
    pPlayer player   ;  // --玩家 0: 無人(銀行)、1~4: 玩家
    pList block      ;  // --板塊的list head
    pList node       ;  // --節點的list head
    pList road       ;  // --道路的list head
    sKing roadKing   ;  // --最長道路
    sKing armyKing   ;  // --最大騎士
} sGame;
*/
#include "catan.h"

int32_t init(){// 初始化遊戲
  PRINTL("Initializing game...");
  // 初始化亂數種子
  srand(time(NULL));

  // 初始化遊戲
  game = (pGame)malloc(sizeof(sGame));

  // 初始化遊戲狀態
  game->state = START;

  // 初始化輪到誰
  game->turn = 1;

  // 初始化骰子點數
  game->dice[0] = game->dice[1] = 0;

  // 初始化玩家
  game->player = NULL;
  if(initPlayer()) return -1;
  PRINTL("Player initialized");

  // 初始化板塊
  game->block = NULL;
  if(initBlock()) return -2;
  PRINTL("Block initialized");

  // 初始化節點
  game->node = NULL;
  if(initNode()) return -3;
  PRINTL("Node initialized");

  // 初始化道路
  game->road = NULL;
  if(initRoad()) return -4;
  PRINTL("Road initialized");

  // 初始化港口
  if(initHarbor()) return -5;
  PRINTL("Harbor initialized");

  //初始化地圖，將板塊 節點 道路連結起來
  if(initMap()) return -5;
  PRINTL("Map initialized");

  //隨機化地圖板塊資源、數字，以及港口
  if(randMap()) return -6;
  PRINTL("Map randomized");

   // 初始化robber
  game->robber = NULL;
  forList(game->block, node){
    pBlock block = entry(node, sBlock);
    if(block->resource == DESERT){
      game->robber = block;
      break;
    }
  }

  // 初始化最長道路
  game->armyKing.owner = 0;
  game->armyKing.size = 0;

  // 初始化最大騎士
  game->roadKing.owner = 0;
  game->roadKing.size = 0;

  PRINTL("Ready to start!");
  return 0;
}

/*
typedef struct _sPlayer_ { // 玩家
    uint8_t  id;           // --編號 0: 無人(銀行)、1~4: 玩家
    uint8_t  score;        // --玩家分數
    uint8_t *resource;     // --玩家資源 大小為5+1的陣列，分別對應五種資源 第一項為總數
    uint8_t *building;     // --玩家建築物 大小為3的陣列，分別對應三種建築物
    uint8_t *harbor;       // --玩家港口 大小為6的陣列，分別對應六種港口。0表示沒有擁有
    uint8_t  roadlength;   // --玩家道路數量
    uint8_t  armySize;     // --玩家軍隊數量
    pList    devcard;      // --玩家發展卡
} sPlayer;
*/

int32_t initPlayer(){
  uint8_t maxPlayer  = 4;  // 4個玩家
  uint8_t maxres     = 19; // 19個資源卡每種 
  uint8_t resType    = 5;  // 5種資源
  uint8_t buildType  = 3;  // 3種建築物
  uint8_t harborType = 6;  // 6種港口
  
  //初始化玩家
  game->player = (pPlayer)malloc(sizeof(sPlayer) * (maxPlayer + 1));
  for(uint8_t i = 0; i <= maxPlayer; i++){
    game->player[i].id         = i;
    game->player[i].score      = 0;
    game->player[i].resource   = (uint8_t*)calloc(resType + 1, sizeof(uint8_t));
    game->player[i].building   = (uint8_t*)calloc(buildType, sizeof(uint8_t));
    game->player[i].harbor     = (uint8_t*)calloc(harborType, sizeof(uint8_t));
    game->player[i].roadlength = 0;
    game->player[i].armySize   = 0;
    game->player[i].devcard    = NULL;
  }

  //全部資源歸到銀行
  for(uint8_t i = 0; i < resType; i++){
    game->player[0].resource[i] = maxres;
  }
  //全部港口歸到銀行
  game->player[0].harbor[0] = 3; // 三個一般港口
  for(uint8_t i = 1; i < harborType; i++){
    game->player[0].harbor[i] = 1;
  }
  //*debug print 顯示銀行資源
  // PRINTL("bank resource:");
  // for(uint8_t i = 0; i < resType; i++){
  //   PRINTL("resType: %c, amount: %d", i , game->player[0].resource[i]);
  // }
  //*debug print 顯示銀行港口
  // PRINTL("bank harbor:");
  // for(uint8_t i = 0; i < harborType; i++){
  //   PRINTL("Type: %d ", game->player[0].harbor[i]);
  // }

  //發展卡linklist初始化
  for(uint8_t i = 0; i <= maxPlayer; i++){
    game->player[i].devcard = initList();
  }
  //創造發展卡，並且放到銀行
  for(uint8_t i = 0; i < 25; i++){ // 25張發展卡
    sDevcard *newcard = (sDevcard*)malloc(sizeof(sDevcard));
    if(i < 14){ // 14張騎士卡
      newcard->type = KNIGHT;
    }else if(i < 19){ // 5張勝利點卡
      newcard->type = VICTORY_POINT;
    }else if(i < 21){ // 2張道路建設卡
      newcard->type = ROAD_BUILDING;
    }else if(i < 23){ // 2張壟斷卡
      newcard->type = YEAR_OF_PLENTY;
    }else{ //2張豐收卡
      newcard->type = MONOPOLY;
    }
    newcard->status = 0;
    push(game->player[0].devcard, &newcard->list, BACK);
  }
  //*debug print
  // forList(game->player[0].devcard, element){
  //   sDevcard *card = entry(element, sDevcard);
  //   PRINTL("devCard %ld: type: %d, status: %d", card->list.index, card->type, card->status);
  //   //sleep(1);
  // }
  //洗牌
  //! 這裡有問題，洗牌後的順序不對
  // PRINTL("problem here!");
  // PRINTL("shuffle devcard...");
  // shuffle(game->player[0].devcard);
  // //*debug print
  // forList(game->player[0].devcard, element){
  //   sDevcard *card = entry(element, sDevcard);
  //   PRINTL("devCard %ld: type: %d, status: %d", card->list.index, card->type, card->status);
  //   sleep(1);
  // }

  return 0;
}

/*
typedef struct _sBlock_ {  // 板塊
    uint8_t   coord[2];     // --坐標 0~4 (row)、0~5 (col)
    uint8_t   number;      // --對應骰子 
    uint8_t   resource;      // --對應資源 
    pNode     node[6];     // --板塊上的節點 由正上方開始順時針計算 
    pBlock    block[6];    // --鄰近的板塊 由正上方開始順時針計算
    sList     list;
} sBlock;
*/
int32_t initBlock(){
  uint8_t minCol = 3;

  //板塊link head初始化
  game->block = initList();
  //創造板塊
  for(uint8_t i = 0; i < 5 ; i++){
    for(uint8_t j = 0 ; j < 5 ; j++){
      if((i == 0 || i == 4) && j >= minCol){
        continue;
      }else if((i == 1 || i == 3) && j >= minCol + 1){
        continue;
      }else if(i == 2 && j >= minCol + 2){//中間那排 暫時沒影響
        continue;
      }
      pBlock newblock = (pBlock)malloc(sizeof(sBlock));
      newblock->coord[0] = i;
      newblock->coord[1] = j;
      newblock->number   = 0;
      newblock->resource   = 0;
      for(uint8_t k = 0; k < 6; k++){
        newblock->node[k]  = NULL;
        newblock->block[k] = NULL;
      }
      push(game->block, &newblock->list, BACK);
    }
  }
  //*debug print
  // forList(game->block, element){
  //   pBlock block = entry(element, sBlock);
  //   PRINTL("block %ld: coord: %d, %d, number: %d, resource: %d", block->list.index, block->coord[0], block->coord[1], block->number, block->resource);
  //   //sleep(1);
  // }
  return 0;
}

/*
typedef struct _sNode_ {   // 節點
    uint8_t coord[2];      // --坐標 0~5 (row)、0~10 (col)
    uint8_t owner;         // --節點擁有者 (0: 無人、1~4: 玩家)
    uint8_t building;      // --建築物種類 (0: 無、1: 村莊、2: 城市)
    pBlock  block[3];      // --節點三個角落的板塊
    pNode   node[3];       // --節點三個角落的節點
    pRoad   road[3];       // --節點三條邊的道路
    pHarbor harbor;        // --節點的港口 (若有)
    sList   list;
} sNode;
*/
int32_t initNode(){

  uint8_t minCol = 7;
  //節點link head初始化
  game->node = initList();
  //創造節點
  for(uint8_t i = 0; i < 6; i++){
    for(uint8_t j = 0; j < 11; j++){
      if((i == 0 || i == 5) && j >= minCol){
        continue;
      }else if((i == 1 || i == 4) && j >= minCol + 2){
        continue;
      }else if((i == 2 || i == 3) && j >= minCol + 4){ //中間那兩排 暫時沒影響
        continue;
      }
      pNode newnode = (pNode)malloc(sizeof(sNode));
      newnode->coord[0] = i;
      newnode->coord[1] = j;
      newnode->owner    = 0;
      newnode->building = 0;
      for(uint8_t k = 0; k < 3; k++){
        newnode->block[k] = NULL;
        newnode->node[k]  = NULL;
        newnode->road[k]  = NULL;
      }
      newnode->harbor   = NULL;
      push(game->node, &newnode->list, BACK);
    }
  }
  //*debug print
  // forList(game->node, element){
  //   pNode node = entry(element, sNode);
  //   PRINTL("node %ld: coord: %d, %d, owner: %d, building: %d", node->list.index, node->coord[0], node->coord[1], node->owner, node->building);
  //   //sleep(1);
  // }
  return 0;
}

/*
typedef struct _sRoad_ {   // 道路
    uint8_t coord[2];      // --坐標 0~10 (row even: 橫的道路;odd: 直的道路)、0~9 (col)
    uint8_t owner;         // --道路擁有者 (0: 無人、1~4: 玩家)
    pNode   node[2];       // --道路兩端的節點
    sList   list;
} sRoad;
*/
int32_t initRoad(){

  //道路link head初始化
  game->road = initList();
  //創造道路
  for(uint8_t i = 0; i < 11; i++){
    uint8_t minCol = 0;
    switch(i/2){
      case 0: case 5:
        minCol = 6;
        break;
      case 1: case 4:
        minCol = 8;
        break;
      case 2: case 3:
        minCol = 10;
        break;
    }
    if(i % 2){
      minCol = minCol / 2 + 1;
      if(i>6) minCol--;
    }
    for(uint8_t j = 0; j < minCol; j++){
      pRoad newroad = (pRoad)malloc(sizeof(sRoad));
      newroad->coord[0] = i;
      newroad->coord[1] = j;
      newroad->owner    = 0;
      for(uint8_t k = 0; k < 2; k++){
        newroad->node[k] = NULL;
      }
      push(game->road, &newroad->list, BACK);
    }
  }
  //*debug print
  // forList(game->road, element){
  //   pRoad road = entry(element, sRoad);
  //   PRINTL("road %ld: coord: %d, %d, owner: %d", road->list.index, road->coord[0], road->coord[1], road->owner);
  //   //sleep(1);
  // }
  return 0;
}

/*
typedef struct _sHarbor_ { // 港口
    uint8_t owner;         // --擁有者 (0: 無人、1~4: 玩家)
    uint8_t type;          // --港口種類 (0: 三換一、1~5: 對應資源二換一)
    pNode   node[2];       // --港口所在的節點
    sList   list;
} sHarbor;
*/
int32_t initHarbor(){
  //港口link head初始化
  game->harbor = initList();
  //創造港口
  for(uint8_t i = 0; i < 9; i++){
    pHarbor newharbor = (pHarbor)malloc(sizeof(sHarbor));
    newharbor->owner    = 0;
    newharbor->type     = 0;
    newharbor->node[0]  = NULL;
    newharbor->node[1]  = NULL;
    push(game->harbor, &newharbor->list, BACK);
  }
  //*debug print
  // forList(game->harbor, element){
  //   pHarbor harbor = entry(element, sHarbor);
  //   PRINTL("harbor %ld: owner: %d, type: %d", harbor->list.index, harbor->owner, harbor->type);
  //   //sleep(1);
  // }
  return 0;
}

//將板塊、節點、道路、港口連結起來
int32_t initMap(){
  //板塊互相連結
  int index = 0, index2 = 0;
  forList(game->block, elememt){//連接水平方向
    index++;
    //跳過最右邊的板塊
    if(index == 3 || index == 7 || index == 12 || index == 16 || index == 19 ){
      continue;
    }
    pBlock blockptr = entry(elememt, sBlock);
    pBlock blocknext = entry(elememt->NEXT, sBlock);
    //左右連接
    blockptr->block[1] = blocknext;
    blocknext->block[4] = blockptr;
  }
  // //*debug print
  // PRINTL("block link:test1")
  // forList(game->block, elememt){
  //   pBlock blockptr = entry(elememt, sBlock);
  //   PRINTL("block %ld: ", blockptr->list.index);
  //   for(int i = 0; i < 6; i++){
  //     if(blockptr->block[i] != NULL){
  //       PRINTL("  block %ld: ", blockptr->block[i]->list.index);
  //     }
  //   }
  //   //sleep(1);
  // }
  index = -1;
  forList(game->block, elememt){//連接ㄑ方向
    index++;
    //跳過板塊
    if(index == 11 || index > 14){
      continue;
    }
    index2 = index ;
    forList(elememt, elememt2){
      index2++;
      //跳過位置不對的板塊
      if(index < 3 ){
        if(index2 != index + 3){
          continue;
        }
      }else if(index < 7){
        if(index2 != index + 4){
          continue;
        }
      }else if(index < 11){
        if(index2 != index + 5){
          continue;
        }
      }else if(index < 15){
        if(index2 != index + 4){
          continue;
        }
      }else if(index < 19){
        if(index2 != index + 3){
          continue;
        }
      }
      //*debug print
      //PRINTL("index: %d, index2: %d", index, index2);
      pBlock blockptr = entry(elememt, sBlock);
      pBlock blocknext = entry(elememt2, sBlock);
      //連接
      if(index < 7){
        blockptr->block[3] = blocknext;
        blocknext->block[0] = blockptr;
      }else{
        blockptr->block[2] = blocknext;
        blocknext->block[5] = blockptr;
      }
    }
  }
  // //*debug print
  // PRINTL("block link:test1")
  // forList(game->block, elememt){
  //   pBlock blockptr = entry(elememt, sBlock);
  //   PRINTL("block %ld: ", blockptr->list.index);
  //   for(int i = 0; i < 6; i++){
  //     if(blockptr->block[i] != NULL){
  //       PRINTL("  block %ld: ", blockptr->block[i]->list.index);
  //     }
  //   }
  //   //sleep(1);
  // }
  index = -1;
  forList(game->block, elememt){//連接＞方向
    index++;
    //跳過板塊
    if(index == 7 || index == 12 || index > 15){
      continue;
    }
    index2 = index ;
    forList(elememt, elememt2){
      index2++;
      //跳過位置不對的板塊
      if(index < 3 ){
        if(index2 != index + 4 ) continue;
      }else if (index < 7 ){
        if(index2 != index + 5 ) continue;
      }else if(index < 12 ){
        if(index2 != index + 4 ) continue;
      }else if(index < 16 ){
        if(index2 != index + 3 ) continue;
      }
      pBlock blockptr = entry(elememt, sBlock);
      pBlock blocknext = entry(elememt2, sBlock);
      //連接
      if(index < 7){
        blockptr->block[2] = blocknext;
        blocknext->block[5] = blockptr;
      }else{
        blockptr->block[3] = blocknext;
        blocknext->block[0] = blockptr;
      }
    }
  }
  // //*debug print
  // PRINTL("block link:test1")
  // forList(game->block, elememt){
  //   pBlock blockptr = entry(elememt, sBlock);
  //   PRINTL("block %ld: ", blockptr->list.index);
  //   for(int i = 0; i < 6; i++){
  //     if(blockptr->block[i] != NULL){
  //       PRINTL("  block %ld: ", blockptr->block[i]->list.index);
  //     }
  //   }
  //   //sleep(1);
  // }

  //節點連接
  forList(game->node, elememt){//橫向連接
    pNode nodeptr = entry(elememt, sNode);
    if(elememt->NEXT == game->node ) continue;
    pNode nodenext = entry(elememt->NEXT, sNode);
    if(nodenext->coord[1] == 0) continue;
    // //*debug print
    // PRINTL("node %ld: ", nodeptr->list.index);
    forList(game->road, elememt2){
      pRoad roadptr = entry(elememt2, sRoad);
      // //*debug print
      // PRINTL("road %ld: ", roadptr->list.index);
      // PRINTL("node coord: %d, %d", nodeptr->coord[0], nodeptr->coord[1]);
      // PRINTL("nodenext coord: %d, %d", nodenext->coord[0], nodenext->coord[1]);
      if(roadptr->coord[0] == nodeptr->coord[0] * 2 && roadptr->coord[1] == nodeptr->coord[1]){
        //PRINTL("node %ld: ", nodeptr->list.index);
        //連接
        nodeptr->node[1] = nodenext;
        nodenext->node[0] = nodeptr;
        //連接道路
        nodeptr->road[1] = roadptr;
        roadptr->node[0] = nodeptr;
        nodenext->road[0] = roadptr;
        roadptr->node[1] = nodenext;
        break;
      }
    }
  }
  //*debug print
  // PRINTL("node link:test1")
  // forList(game->node, elememt){
  //   pNode nodeptr = entry(elememt, sNode);
  //   PRINTL("node %ld: ", nodeptr->list.index);
  //   for(int i = 0; i < 2; i++){
  //     if(nodeptr->node[i] != NULL){
  //       PRINTL("  node %ld: ", nodeptr->node[i]->list.index);
  //     }
  //   }
  //   //sleep(1);
  // }
  // PRINTL("road link:test1")
  // forList(game->road, elememt){
  //   pRoad roadptr = entry(elememt, sRoad);
  //   PRINTL("road %ld: coord: %d, %d", roadptr->list.index, roadptr->coord[0], roadptr->coord[1]);
  //   for(int i = 0; i < 2; i++){
  //     if(roadptr->node[i] != NULL){
  //       PRINTL("  node %ld: ", roadptr->node[i]->list.index);
  //     }
  //   }
  //   //sleep(1);
  // }

  forList(game->node, elememt){//直向連接
    pNode nodeptr = entry(elememt, sNode);
    if(nodeptr->coord[0] == 5){
      continue;
    }
    if(nodeptr->coord[0] <2 && nodeptr->coord[1]%2 ==0){
      forList(game->node->NEXT, elememt2){
        pNode nodenext = entry(elememt2, sNode);
        if(nodenext->coord[0] == nodeptr->coord[0] +1 && nodenext->coord[1] == nodeptr->coord[1] +1){
          forList(game->road, elememt3){
            pRoad roadptr = entry(elememt3, sRoad);
            if(roadptr->coord[0] == nodeptr->coord[0] * 2 + 1 && roadptr->coord[1] * 2 == nodeptr->coord[1]){
              //連接
              nodeptr->node[2] = nodenext;
              nodenext->node[2] = nodeptr;
              //連接道路
              nodeptr->road[2] = roadptr;
              roadptr->node[0] = nodeptr;
              nodenext->road[2] = roadptr;
              roadptr->node[1] = nodenext;
              break;
            }
          }
          break;
        }
      }
    }else if(nodeptr->coord[0] ==2 && nodeptr->coord[1]%2 ==0){
      //PRINTL("node %ld: ", nodeptr->list.index);
      forList(game->node->NEXT, elememt2){
        pNode nodenext = entry(elememt2, sNode);
        //PRINTL("nodenext %ld: ", nodenext->list.index);
        if(nodenext->coord[0] == nodeptr->coord[0] +1 && nodenext->coord[1] == nodeptr->coord[1]){     
          //PRINTL("node %ld: ", nodeptr->list.index);
          forList(game->road, elememt3){
            pRoad roadptr = entry(elememt3, sRoad);
            if(roadptr->coord[0] == nodeptr->coord[0] * 2 + 1 && roadptr->coord[1] * 2 == nodeptr->coord[1]){
              //*debug print
              //連接
              nodeptr->node[2] = nodenext;
              nodenext->node[2] = nodeptr;
              //連接道路
              nodeptr->road[2] = roadptr;
              roadptr->node[0] = nodeptr;
              nodenext->road[2] = roadptr;
              roadptr->node[1] = nodenext;
              break;
            }
          }
          break;
        }
      }
    }else if(nodeptr->coord[0] >2 && nodeptr->coord[1]%2 ==1){
      forList(game->node->NEXT, elememt2){
        pNode nodenext = entry(elememt2, sNode);
        if(nodenext->coord[0] == nodeptr->coord[0] +1 && nodenext->coord[1] == nodeptr->coord[1] -1){
          forList(game->road, elememt3){
            pRoad roadptr = entry(elememt3, sRoad);
            if(roadptr->coord[0] == nodeptr->coord[0] * 2 + 1 && roadptr->coord[1] * 2 +1 == nodeptr->coord[1] ){
              //連接
              nodeptr->node[2] = nodenext;
              nodenext->node[2] = nodeptr;
              //連接道路
              nodeptr->road[2] = roadptr;
              roadptr->node[0] = nodeptr;
              nodenext->road[2] = roadptr;
              roadptr->node[1] = nodenext;
              break;
            }
          }
          break;
        }
      }
    }
  }
  //*debug print
  // PRINTL("node link:test1")
  // forList(game->node, elememt){
  //   pNode nodeptr = entry(elememt, sNode);
  //   PRINTL("node %ld: coord: %d, %d", nodeptr->list.index, nodeptr->coord[0], nodeptr->coord[1]);
  //   for(int i = 0; i < 3; i++){
  //     if(nodeptr->node[i] != NULL){
  //       PRINTL("i:%d  node %ld: coord: %d, %d", i, nodeptr->node[i]->list.index, nodeptr->node[i]->coord[0], nodeptr->node[i]->coord[1]);
  //     }
  //   }
  //   //sleep(1);
  // }
  // PRINTL("road link:test1")
  // forList(game->road, elememt){
  //   pRoad roadptr = entry(elememt, sRoad);
  //   PRINTL("road %ld: coord: %d, %d", roadptr->list.index, roadptr->coord[0], roadptr->coord[1]);
  //   for(int i = 0; i < 2; i++){
  //     if(roadptr->node[i] != NULL){
  //       PRINTL("  node %ld: coord: %d, %d", roadptr->node[i]->list.index, roadptr->node[i]->coord[0], roadptr->node[i]->coord[1]);
  //     }
  //   }
  //   //sleep(1);
  // }

  //港口連接
  index =-1;
  forList(game->harbor, elememt){
    index++;
    pHarbor harborptr = entry(elememt, sHarbor);
    forList(game->node, elememt2){
      pNode nodeptr = entry(elememt2, sNode);
      if(index == 0 && nodeptr->node[1] != NULL ){
        if(nodeptr->coord[0] == 0 && nodeptr->coord[1] == 0 && nodeptr->node[1]->coord[0] == 0 && nodeptr->node[1]->coord[1] == 1){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[1];
          nodeptr->node[1]->harbor = harborptr;
          break;
        }
      }else if(index == 1 && nodeptr->node[1] != NULL){
        if(nodeptr->coord[0] == 0 && nodeptr->coord[1] == 3 && nodeptr->node[1]->coord[0] == 0 && nodeptr->node[1]->coord[1] == 4){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[1];
          nodeptr->node[1]->harbor = harborptr;
          break;
        }
      }else if(index == 2 && nodeptr->node[1] != NULL){
        if(nodeptr->coord[0] == 1 && nodeptr->coord[1] == 7 && nodeptr->node[1]->coord[0] == 1 && nodeptr->node[1]->coord[1] == 8){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[1];
          nodeptr->node[1]->harbor = harborptr;
          break;
        }
      }else if(index == 3 && nodeptr->node[2] != NULL){
        if(nodeptr->coord[0] == 1 && nodeptr->coord[1] == 0 && nodeptr->node[2]->coord[0] == 2 && nodeptr->node[2]->coord[1] == 1){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[2];
          nodeptr->node[2]->harbor = harborptr;
          break;
        }
      }else if(index == 4 && nodeptr->node[2] != NULL){
        if(nodeptr->coord[0] == 2 && nodeptr->coord[1] == 10 && nodeptr->node[2]->coord[0] == 3 && nodeptr->node[2]->coord[1] == 10){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[2];
          nodeptr->node[2]->harbor = harborptr;
          break;
        }
      }else if(index == 5 && nodeptr->node[2] != NULL){
        if(nodeptr->coord[0] == 3 && nodeptr->coord[1] == 1 && nodeptr->node[2]->coord[0] == 4 && nodeptr->node[2]->coord[1] == 0){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[2];
          nodeptr->node[2]->harbor = harborptr;
          break;
        }
      }else if(index == 6 && nodeptr->node[1] != NULL){
        if(nodeptr->coord[0] == 4 && nodeptr->coord[1] == 7 && nodeptr->node[1]->coord[0] == 4 && nodeptr->node[1]->coord[1] == 8){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[1];
          nodeptr->node[1]->harbor = harborptr;
          break;
        }
      }else if(index == 7 && nodeptr->node[1] != NULL){
        if(nodeptr->coord[0] == 5 && nodeptr->coord[1] == 0 && nodeptr->node[1]->coord[0] == 5 && nodeptr->node[1]->coord[1] == 1){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[1];
          nodeptr->node[1]->harbor = harborptr;
          break;
        }
      }else if(index == 8 && nodeptr->node[1] != NULL){
        if(nodeptr->coord[0] == 5 && nodeptr->coord[1] == 3 && nodeptr->node[1]->coord[0] == 5 && nodeptr->node[1]->coord[1] == 4){
          harborptr->node[0] = nodeptr;
          nodeptr->harbor = harborptr;
          harborptr->node[1] = nodeptr->node[1];
          nodeptr->node[1]->harbor = harborptr;
          break;
        }
      }
    }
  }
  //*debug print
  // PRINTL("harbor link:test1")
  // forList(game->harbor, elememt){
  //   pHarbor harborptr = entry(elememt, sHarbor);
  //   PRINTL("harbor %ld; type: %d; node1: %d, %d; node2: %d, %d", harborptr->list.index, harborptr->type, harborptr->node[0]->coord[0], harborptr->node[0]->coord[1], harborptr->node[1]->coord[0], harborptr->node[1]->coord[1]);
  //   //sleep(1);
  // }

  //節點與板塊連接
  index = -1;
  forList(game->block, element){//連上半部
    pBlock blockptr = entry(element, sBlock);
    index++;
    forList(game->node, element2){
      pNode nodeptr = entry(element2, sNode);
      if(blockptr->coord[0] < 2){
        if(blockptr->coord[0] == nodeptr->coord[0] ){
          if(blockptr->coord[1] * 2 == nodeptr->coord[1]){
            blockptr->node[5] = nodeptr;
            nodeptr->block[1] = blockptr;
          }else if(blockptr->coord[1] * 2 + 1 == nodeptr->coord[1]){
            blockptr->node[0] = nodeptr;
            nodeptr->block[2] = blockptr;
          }else if(blockptr->coord[1] * 2 + 2 == nodeptr->coord[1]){
            blockptr->node[1] = nodeptr;
            nodeptr->block[0] = blockptr;
          }
        }else if(blockptr->coord[0] +1 == nodeptr->coord[0]){
          if(blockptr->coord[1] * 2 + 1== nodeptr->coord[1]){
            blockptr->node[4] = nodeptr;
            nodeptr->block[1] = blockptr;
          }else if(blockptr->coord[1] * 2 + 2 == nodeptr->coord[1]){
            blockptr->node[3] = nodeptr;
            nodeptr->block[2] = blockptr;
          }else if(blockptr->coord[1] * 2 + 3 == nodeptr->coord[1]){
            blockptr->node[2] = nodeptr;
            nodeptr->block[0] = blockptr;
          }
        }
      }else if(blockptr->coord[0] > 2){
        if(blockptr->coord[0] == nodeptr->coord[0] ){
          if(blockptr->coord[1] * 2 + 1 == nodeptr->coord[1]){
            blockptr->node[5] = nodeptr;
            nodeptr->block[1] = blockptr;
          }else if(blockptr->coord[1] * 2 + 2 == nodeptr->coord[1]){
            blockptr->node[0] = nodeptr;
            nodeptr->block[2] = blockptr;
          }else if(blockptr->coord[1] * 2 + 3 == nodeptr->coord[1]){
            blockptr->node[1] = nodeptr;
            nodeptr->block[0] = blockptr;
          }
        }else if(blockptr->coord[0] + 1 == nodeptr->coord[0]){
          if(blockptr->coord[1] * 2 == nodeptr->coord[1]){
            blockptr->node[4] = nodeptr;
            nodeptr->block[1] = blockptr;
          }else if(blockptr->coord[1] * 2 + 1 == nodeptr->coord[1]){
            blockptr->node[3] = nodeptr;
            nodeptr->block[2] = blockptr;
          }else if(blockptr->coord[1] * 2 + 2 == nodeptr->coord[1]){
            blockptr->node[2] = nodeptr;
            nodeptr->block[0] = blockptr;
          }
        }
      }else{
        if(blockptr->coord[0] == nodeptr->coord[0] ){
          if(blockptr->coord[1] * 2 == nodeptr->coord[1]){
            blockptr->node[5] = nodeptr;
            nodeptr->block[1] = blockptr;
          }else if(blockptr->coord[1] * 2 + 1 == nodeptr->coord[1]){
            blockptr->node[0] = nodeptr;
            nodeptr->block[2] = blockptr;
          }else if(blockptr->coord[1] * 2 + 2 == nodeptr->coord[1]){
            blockptr->node[1] = nodeptr;
            nodeptr->block[0] = blockptr;
          }
        }else if(blockptr->coord[0] +1 == nodeptr->coord[0]){
          if(blockptr->coord[1] * 2 == nodeptr->coord[1]){
            blockptr->node[4] = nodeptr;
            nodeptr->block[1] = blockptr;
          }else if(blockptr->coord[1] * 2 + 1 == nodeptr->coord[1]){
            blockptr->node[3] = nodeptr;
            nodeptr->block[2] = blockptr;
          }else if(blockptr->coord[1] * 2 + 2 == nodeptr->coord[1]){
            blockptr->node[2] = nodeptr;
            nodeptr->block[0] = blockptr;
          }
        }
      }
    }
  }
  //*debug print
  // PRINTL("block link:test1")
  // forList(game->block, elememt){
  //   pBlock blockptr = entry(elememt, sBlock);
  //   for(int i = 0; i < 6; i++){
  //     PRINTL("block %ld; node %d: %d, %d", blockptr->list.index, i, blockptr->node[i]->coord[0], blockptr->node[i]->coord[1]);
  //   }
  // }
  // PRINTL("node link:test1");
  // forList(game->node, elememt){
  //   pNode nodeptr = entry(elememt, sNode);
  //   for(int i = 0; i < 3; i++){
  //     if(nodeptr->block[i] != NULL){
  //       PRINTL("node %ld; block %d: %d, %d", nodeptr->list.index, i, nodeptr->block[i]->coord[0], nodeptr->block[i]->coord[1]);
  //     }
  //   }
  // }

  return 0;
}
