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

int32_t init(){
  //TODO: calloc
  game = (pGame)malloc(sizeof(sGame));

  // 初始化遊戲狀態
  game->state = START;

  // 初始化輪到誰
  game->turn = 1;

  // 初始化骰子點數
  game->dice[0] = game->dice[1] = 0;

  // 初始化玩家
  game->player = NULL;
  if(init_player()) return -1;

  // 初始化板塊
  game->block = NULL;
  if(init_block()) return -2;

  // 初始化節點
  game->node = NULL;
  if(init_node()) return -3;

  // 初始化道路
  game->road = NULL;
  if(init_road()) return -4;

  //初始化地圖，將板塊 節點 道路連結起來
  if(init_map()) return -5;

  //隨機化地圖板塊資源、數字，以及港口
  if(init_map_randomize()) return -6;

  // 初始化最長道路
  game->armyKing.owner = 0;
  game->armyKing.size = 0;

  // 初始化最大騎士
  game->roadKing.owner = 0;
  game->roadKing.size = 0;

  return 0;
}

/*
typedef struct _sPlayer_ { // 玩家
    uint8_t  id;           // --編號 0: 無人(銀行)、1~4: 玩家
    uint8_t  score;        // --玩家分數
    uint8_t *resource;     // --玩家資源 大小為5+1的陣列，分別對應五種資源 第一項為總數 
    uint8_t *building;     // --玩家建築物 大小為3的陣列，分別對應三種建築物 一開始都是0
    uint8_t *harbor;       // --玩家港口 大小為6的陣列，分別對應六種港口。等於1表示有、0表示無
    uint8_t  roadlength;   // --玩家道路數量
    uint8_t  armySize;     // --玩家軍隊數量
    pList    devcard;      // --玩家發展卡
} sPlayer;
*/

int32_t init_player(){
  uint8_t maxPlayer  = 4;  // 4個玩家
  uint8_t maxres     = 19; // 19個資源卡每種 
  uint8_t resType    = 5;  // 5種資源
  uint8_t buildType  = 3;  // 3種建築物
  uint8_t harborType = 6;  // 6種港口
  
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
    push(game->player[0].devcard, &newcard->list, FRONT);
  }

  //洗牌
  shuffle(game->player[0].devcard);
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
int32_t init_block(){
  //板塊link head初始化
  game->block = initList();

  uint8_t minCol = 3;
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
      push(game->block, &newblock->list, FRONT);
    }
  }
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
int32_t init_node(){
  //節點link head初始化
  game->node = initList();

  uint8_t minCol = 7;
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
      push(game->node, &newnode->list, FRONT);
    }
  }
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
int32_t init_road(){
  //道路link head初始化
  game->road = initList();

  uint8_t minCol = 4;
  //創造道路
  for(uint8_t i = 0; i < 11; i++){
    for(uint8_t j = 0; j < 10; j++){
      if((i % 2 == 0) && j >= minCol){
        continue;
      }else if((i % 2 == 1) && j >= minCol + 1){
        continue;
      }

      pRoad newroad = (pRoad)malloc(sizeof(sRoad));
      newroad->coord[0] = i;
      newroad->coord[1] = j;
      newroad->owner    = 0;
      for(uint8_t k = 0; k < 2; k++){
        newroad->node[k] = NULL;
      }
      push(game->road, &newroad->list, FRONT);
    }
  }
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
int32_t init_harbor(){
  //港口link head初始化
  game->harbor = initList();

  //創造港口
  for(uint8_t i = 0; i < 9; i++){
    pHarbor newharbor = (pHarbor)malloc(sizeof(sHarbor));
    newharbor->owner    = 0;
    newharbor->type     = 0;
    newharbor->node[0]  = NULL;
    newharbor->node[1]  = NULL;
    push(game->harbor, &newharbor->list, FRONT);
  }
  return 0;
}

//將板塊、節點、道路、港口連結起來
int32_t init_map(){
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

  index = 0;
  forList(game->block, elememt){//連接ㄑ方向
    index++;
    //跳過板塊
    if(index == 12 || index > 15){
      continue;
    }
    index2 = index ;
    forList(game->block->NEXT, elememt2){
      index2++;
      //跳過位置不對的板塊
      if(index < 4 && index2 != index +3){
        continue;
      }else if (index < 8 && index2 != index +4){
        continue;
      }else if(index < 13 && index2 != index +5){
        continue;
      }else if(index < 17 && index2 != index +4){
        continue;
      }
      pBlock blockptr = entry(elememt, sBlock);
      pBlock blocknext = entry(elememt2, sBlock);
      //連接
      if(index < 8){
        blockptr->block[3] = blocknext;
        blocknext->block[0] = blockptr;
      }else{
        blockptr->block[2] = blocknext;
        blocknext->block[5] = blockptr;
      }
    }
  }

  index = 0;
  forList(game->block, elememt){//連接＞方向
    index++;
    //跳過板塊
    if(index == 8 || index == 13 || index > 16){
      continue;
    }
    index2 = index ;
    forList(game->block->NEXT, elememt2){
      index2++;
      //跳過位置不對的板塊
      if(index < 3 && index2 != index +4){
        continue;
      }else if (index < 7 && index2 != index +5){
        continue;
      }else if(index < 12 && index2 != index +4){
        continue;
      }else if(index < 16 && index2 != index +3){
        continue;
      }
      pBlock blockptr = entry(elememt, sBlock);
      pBlock blocknext = entry(elememt2, sBlock);
      //連接
      if(index < 8){
        blockptr->block[2] = blocknext;
        blocknext->block[5] = blockptr;
      }else{
        blockptr->block[3] = blocknext;
        blocknext->block[0] = blockptr;
      }
    }
  }


  //節點連接
  forList(game->node, elememt){//橫向連接
    pNode nodeptr = entry(elememt, sNode);
    if(elememt->NEXT == game->node ){
      continue;
    }
    pNode nodenext = entry(elememt->NEXT, sNode);
    if(nodenext->coord[1] == 0){
      continue;
    }
    forList(game->road, elememt2){
      pRoad roadptr = entry(elememt2, sRoad);
      if(roadptr->coord[0] == nodeptr->coord[0] * 2 && roadptr->coord[1] == nodeptr->coord[1]){
        //連接
        nodeptr->node[1] = nodenext;
        nodenext->node[0] = nodeptr;
        //連接道路
        nodeptr->road[1] = roadptr;
        roadptr->node[0] = nodeptr;
        nodenext->road[0] = roadptr;
        roadptr->node[1] = nodenext;
      }
      break;
    }
  }

  forList(game->node, elememt){//直向連接
    pNode nodeptr = entry(elememt, sNode);
    if(nodeptr->coord[0] == 5){
      continue;
    }
    if(nodeptr->coord[0] <3 && nodeptr->coord[1]%2 ==0){
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
    }else if(nodeptr->coord[0] ==3){
      forList(game->node->NEXT, elememt2){
        pNode nodenext = entry(elememt2, sNode);
        if(nodenext->coord[0] == nodeptr->coord[0] +1 && nodenext->coord[1] == nodeptr->coord[1]){
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
    }else{
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

  //港口連接
  index =-1;
  forList(game->harbor, elememt){
    index++;
    pHarbor harborptr = entry(elememt, sHarbor);
    forList(game->node, elememt2){
      pNode nodeptr = entry(elememt2, sNode);
      forList(game->node->NEXT, elememt3){
        pNode nodenext = entry(elememt3, sNode);
        if(index<2){
          if(nodeptr->coord[1] == 0 && (nodeptr->coord[0] == 0 || nodeptr->coord[0] == 5) && nodenext->coord[1] == nodeptr->coord[1] + 1 && nodenext->coord[0] == nodeptr->coord[0]){
            harborptr->node[0] = nodeptr;
            nodeptr->harbor = harborptr;
            harborptr->node[1] = nodenext;
            nodenext->harbor = harborptr;
            break;
          }
        }else if(index<4){
          if(nodeptr->coord[1] == 3 && (nodeptr->coord[0] == 0 || nodeptr->coord[0] == 5) && nodenext->coord[1] == nodeptr->coord[1] + 1 && nodenext->coord[0] == nodeptr->coord[0]){
            harborptr->node[0] = nodeptr;
            nodeptr->harbor = harborptr;
            harborptr->node[1] = nodenext;
            nodenext->harbor = harborptr;
            break;
          }
        }else if(index<6){
          if(nodeptr->coord[1] == 7 && (nodeptr->coord[0] == 1 || nodeptr->coord[0] == 4) && nodenext->coord[1] == nodeptr->coord[1] + 1 && nodenext->coord[0] == nodeptr->coord[0]){
            harborptr->node[0] = nodeptr;
            nodeptr->harbor = harborptr;
            harborptr->node[1] = nodenext;
            nodenext->harbor = harborptr;
            break;
          }
        }else if(index<8){
          if((nodeptr->coord[0] == 1 && nodeptr->coord[1] == 0 && nodenext->coord[0] == 2 && nodenext->coord[1] == 1) || 
              (nodeptr->coord[0]==3 && nodeptr->coord[1] == 1 && nodenext->coord[0] == 4 && nodenext->coord[1] == 0)){
            harborptr->node[0] = nodeptr;
            nodeptr->harbor = harborptr;
            harborptr->node[1] = nodenext;
            nodenext->harbor = harborptr;
            break;
          }
        }else if(index<9){
          if(nodeptr->coord[0] == 2 && nodeptr->coord[1] == 10 && nodenext->coord[0] == 3 && nodenext->coord[1] == 10){
            harborptr->node[0] = nodeptr;
            nodeptr->harbor = harborptr;
            harborptr->node[1] = nodenext;
            nodenext->harbor = harborptr;
            break;
          }
        }
      }
    }
  }

  //節點與板塊連接
  index = -1;
  forList(game->block, element){//連上半部
    pBlock blockptr = entry(element, sBlock);
    index++;
    forList(game->node, element2){
      pNode nodeptr = entry(element2, sNode);
      if(blockptr->coord[0] < 3){
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
      }else if(blockptr->coord[0] > 3){
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

  return 0;
}

int32_t init_map_randomize(){//隨機地圖板塊資源&點數
  int index = 0;
  uint8_t res[19] = {0};
  for(uint8_t i = 0; i < 15; i++){
    res[i] = i % 5 + 1;
  }
  uint8_t random_resource[3] = {0,0,0};
  for(uint8_t i = 1; i < 4; i++){
    random_resource[i-1] = rand() % 5 + 1;
    for(uint8_t j = 0; j < i-1; j++){
      if(random_resource[i-1] == random_resource[j]){
        i--;
        break;
      }
    }
  }
  res[15] = random_resource[0];
  res[16] = random_resource[1];
  res[17] = random_resource[2];
  for(uint8_t j = 0; j < 10; j++){//隨機交換30輪
      for(uint8_t i = 0; i < 19; i++){
      uint8_t random = rand() % 19;
      uint8_t temp = res[i];
      res[i] = res[random];
      res[random] = temp;
    }
  }

  //隨機點數 2~12, 2和12只有一個, 沒有7
  uint8_t random_point[19] = {0,2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12};
  for(uint8_t j = 0; j < 10; j++){//隨機交換30輪
      for(uint8_t i = 0; i < 19; i++){
      uint8_t random = rand() % 19;
      uint8_t temp = random_point[i];
      random_point[i] = random_point[random];
      random_point[random] = temp;
    }
  }
  //賦予地圖板塊資源&點數
  index = 0;
  forList(game->block, element){
    pBlock blockptr = entry(element, sBlock);
    if(blockptr->resource == 0){
      blockptr->resource = random_resource[index];
      blockptr->number = random_point[index];
      index++;
    }
  }

  //港口資源隨機
  uint8_t port_resource[9] = {0,0,0,1,2,3,4,5,6};
  for(uint8_t j = 0; j < 10; j++){//隨機交換30輪
      for(uint8_t i = 0; i < 9; i++){
      uint8_t random = rand() % 9;
      uint8_t temp = port_resource[i];
      port_resource[i] = port_resource[random];
      port_resource[random] = temp;
    }
  }
  //賦予港口資源
  index = 0;
  forList(game->harbor, element){
    pHarbor harborptr = entry(element, sHarbor);
    harborptr->type = port_resource[index];
    index++;
  }

  //確認6 8沒有相鄰
  forList(game->block, element){
    pBlock blockptr = entry(element, sBlock);
    if(blockptr->number == 6 || blockptr->number == 8){
      for(uint8_t i = 0; i < 6; i++){
        if(blockptr->block[i]->number == 6 || blockptr->block[i]->number == 8){
          init_map_randomize();//重新隨機
        }
      }
    }
  }
  return 0;
}
  