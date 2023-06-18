#include"catan.h"

int32_t randMap(){//隨機地圖板塊資源&點數
  int index = 0;
  uint8_t res[19] = {0};
  for(uint8_t i = 0; i < 15; i++){
    res[i] = i % 5 + 1;
  }
  uint8_t random_resource[3] = {0};
  for(uint8_t i = 1; i < 4; i++){
    random_resource[i-1] = rand() % 5 + 1;
    for(uint8_t j = 0; j < i-1; j++){
      if(random_resource[i-1] == random_resource[j]){
        i--;
        break;
      }
    }
    res[14+i] = random_resource[i-1];
  }

  for(uint8_t j = 0; j < 5; j++){//隨機交換5輪
      for(uint8_t i = 0; i < 19; i++){
      uint8_t random = rand() % 19;
      uint8_t temp = res[i];
      res[i] = res[random];
      res[random] = temp;
    }
  }
  //* debug res array
  // for(uint8_t i = 0; i < 19; i++){
  //   PRINTL("%d", res[i]);
  // }


  //隨機點數 2~12, 2和12只有一個, 沒有7
  uint8_t random_point[19] = {0,2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12};
  for(uint8_t j = 0; j < 5; j++){//隨機交換5輪
      for(uint8_t i = 0; i < 19; i++){
      uint8_t random = rand() % 19;
      uint8_t temp = random_point[i];
      random_point[i] = random_point[random];
      random_point[random] = temp;
    }
  }
  //check desert resource and number
  for(uint8_t i = 0; i < 19; i++){
    if(random_point[i] == 0){
      for(uint8_t j = 0; j < 19; j++){
        if(res[j] == 0){
          uint8_t temp = res[i];
          res[i] = res[j];
          res[j] = temp;
          break;
        }
      }
      break;
    }
  }

  //賦予地圖板塊資源&點數
  index = 0;
  forList(game->block, element){
    pBlock blockptr = entry(element, sBlock);
    blockptr->resource = res[index];
    blockptr->number = random_point[index];
    index++;
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
  // //* debug block
  // forList(game->block, element){
  //   pBlock blockptr = entry(element, sBlock);
  //   PRINTL("block %d,%d resource %d number %d, index %ld", blockptr->coord[0], blockptr->coord[1], blockptr->resource, blockptr->number, blockptr->list.index);
  // }
  //確認6 8沒有相鄰
  forList(game->block, element){
    pBlock blockptr = entry(element, sBlock);
    if(blockptr->number == 6 || blockptr->number == 8){
      for(uint8_t i = 0; i < 6; i++){
        if(blockptr->block[i] != NULL){
          pBlock neighborptr = blockptr->block[i];
          if(neighborptr->number == 6 || neighborptr->number == 8){
            randMap();
            break;
          }
        }
      }
    }
  }
  return 0;
}
  
int randPickRoad(){
  forList(game->road, element){
    pRoad roadptr = entry(element, sRoad);
    for(uint8_t i = 0; i < 2; i++){
      if(roadptr->node[i]->owner == game->turn){
        roadptr->owner = game->turn;
        return roadptr->list.index;
      }else if(roadptr->node[i]->owner == NONE ){
        for(uint8_t j = 0; j < 3; j++){
          if(roadptr->node[i]->road[j] == NULL) continue;
          if(roadptr->node[i]->road[j]->owner == game->turn){
            pRoad neighborptr = roadptr->node[i]->road[j];
            if(neighborptr->owner == game->turn){
              roadptr->owner = game->turn;
              return roadptr->list.index;
            }
          }
        }
      }
    }
  }
  return -1;
}

int randPickNode(){
  switch(game->state){
    case SETTLE://random
      uint8_t max = 0;
      pNode maxNode = NULL;
      forList(game->node, element){
        pNode nodeptr = entry(element, sNode);
        uint8_t temp = 0;
        for(uint8_t i = 0 ; i < 3 ; i++){
          if(nodeptr->block[i] == NULL) continue;
          temp += nodeptr->block[i]->number;
        }
        if(temp > max){
          max = temp;
          maxNode = nodeptr;
        }
      }
      maxNode->owner = game->turn;
      return maxNode->list.index;
      break;
    case BUILD://not random
      forList(game->node, element){
        pNode nodeptr = entry(element, sNode);
        if(nodeptr->owner == NONE && 
            (nodeptr->road[0]->owner == game->turn || nodeptr->road[1]->owner == game->turn || nodeptr->road[2]->owner == game->turn)){
          nodeptr->owner = game->turn;
          return nodeptr->list.index;
        }
      }
      break;
    default:
      return -1;
      break;
  }
  return -1;
}

int randDiceNum(){
  //totally random dice
  game->dice[0] = rand() % 6 + 1;
  game->dice[1] = rand() % 6 + 1;
  //* debug dice
  //PRINTL("dice1: %d, dice2: %d\n", game->dice[0], game->dice[1]);
  return 0;
}

int randAction(){

  return 0;
}

int randPickCard(){

  return 0;
}

int randPickResource(){

  return 0;
}

int randLostResource(){

  return 0;
}

int randPickBlock(){

  return 0;
}



