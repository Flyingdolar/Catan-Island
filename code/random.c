#include"catan.h"

int32_t randMap(){//隨機地圖板塊資源&點數
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
          randMap();//重新隨機
        }
      }
    }
  }
  return 0;
}
  
int randPickRoad(){

}

int randPickNode(){

}

int randDiceNum(){
  PRINTL("randomDice\n");
  //totally random dice
  game->dice[0] = rand() % 6 + 1;
  game->dice[1] = rand() % 6 + 1;

  //test dice
  PRINTL("dice1: %d, dice2: %d\n", game->dice[0], game->dice[1]);
  return 0;
}

int randAction(){

}

int randPickCard(){

}

int randPickResource(){

}

int randLostResource(){

}

int randPickBlock(){
  PRINTL("randomRobber\n");
  //totally random robber
  uint8_t coord[2] = {0};
  coord[0] = rand() % 5 ;
  coord[1] = rand() % 5 ;
  if(coord[0] == game->robber[0] && coord[1] == game->robber[1]){
    randPickBlock();
  }else{
    forList(game->block, element){
      pBlock blockptr = entry(element, sBlock);
      if(blockptr->coord[0] == coord[0] && blockptr->coord[1] == coord[1]){
        game->robber[0] = coord[0];
        game->robber[1] = coord[1];
        break;
      }
    }
    if(game->robber[0] == coord[0] && game->robber[1] == coord[1]){
      PRINTL("robber: %d, %d\n", game->robber[0], game->robber[1]);
    }else{
      randPickBlock();
    }
  }

  return 0;
}

int randLostResource(){

}

