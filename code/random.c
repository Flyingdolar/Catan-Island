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
    if(blockptr->number == 0) game->robber = blockptr;
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
  uint8_t player = game->turn;
  pRoad maxValue = NULL;
  uint8_t threshold = 100;
  switch(game->state){
    case SETTLE:
      forList(game->node, element){
        pNode pos = entry(element, sNode);
        if(pos->owner != player) continue;// not my node
        for(uint8_t i = 0; i < 3; i++){
          uint8_t temp = 0;
          if(pos->road[i] != NULL){
            if(pos->road[i]->owner == player) break;// already have road
            if(pos->node[i] != NULL){
              pNode neighbor = pos->node[i];
              for(uint8_t j = 0; j < 3; j++){
                if(neighbor->node[j] != NULL){
                  pNode possible = neighbor->node[j];
                  if(possible->owner != 0 ) continue;// not empty node
                  for(uint8_t k = 0; k < 3; k++){
                    if(possible->block[k] != NULL){
                      temp += abs(possible->block[k]->number - 7 );
                    }else{
                      temp += 7;
                    }
                  }
                  if(temp < threshold){
                    threshold = temp;
                    maxValue = pos->road[i];
                  }
                }
              }
            }
          }
        }
      }
      if(maxValue != NULL) return maxValue->list.index;
      break;
    case BUILD://if cant find best node to build,, random
      forList(game->node, element){
        pNode pos = entry(element, sNode);
        if(pos->owner != player) continue;// not my node
        for(uint8_t i = 0; i < 3; i++){
          uint8_t temp = 0;
          if(pos->road[i] != NULL){
            if(pos->road[i]->owner == player){// already have road

            }else{
              if(pos->node[i] != NULL){
                pNode neighbor = pos->node[i];
                for(uint8_t j = 0; j < 3; j++){
                  if(neighbor->node[j] != NULL){
                    pNode possible = neighbor->node[j];
                    if(possible->owner != 0 ) continue;// not empty node
                    for(uint8_t k = 0; k < 3; k++){
                      if(possible->block[k] != NULL){
                        temp += abs(possible->block[k]->number - 7 );
                      }else{
                        temp += 7;
                      }
                    }
                    if(temp < threshold){
                      threshold = temp;
                      maxValue = pos->road[i];
                    }
                  }
                }
              }
            }
          }
        }
      }
      if(maxValue != NULL) return maxValue->list.index;
      break;
    default:
      break;
  }
  return rand() % game->road->index; 
}

int randPickNode(){
  pNode maxValueNode = NULL;
  uint8_t threshold = 100, temp = 0;
  switch(game->state){
    case SETTLE://not random; find max value node
      forList(game->node, element){
        pNode nodeptr = entry(element, sNode);
        if(nodeptr->owner != 0) continue;// not empty node
        temp = 0;
        for(uint8_t i = 0 ; i < 3; i++){
          if(nodeptr->block[i] != NULL) {
            for(uint8_t j = 0; j < 3; j++){
              if(nodeptr->block[i]->node[j] == NULL) continue;
              if(nodeptr->block[i]->node[j]->owner > 0) break;
            }
            temp += abs(nodeptr->block[i]->number - 7);
          }else{
            temp += 7;
          }
        }
        if(temp < threshold){
          threshold = temp;
          maxValueNode = nodeptr;
        }
      }
      return maxValueNode->list.index;
      break;
    case BUILD://not random
      // forList(game->node, element){
      //   pNode nodeptr = entry(element, sNode);
      //   if(nodeptr->owner == game->turn){
      //     for(uint8_t i = 0 ; i < 3; i++){
      //       if(nodeptr->road[i] == NULL || nodeptr->owner != 0) continue;
      //       if(nodeptr->road[i]->owner == game->turn && nodeptr->owner == game->turn ){
      //         for(uint8_t i = 0 ; i < 3; i++){
      //           if(nodeptr->block[i] != NULL) {
      //             temp += abs(nodeptr->block[i]->number - 7);
      //           }else{
      //             temp += 7;
      //           }
      //         }
      //         if(temp < min){//find most valuable node
      //           min = temp;
      //           minNode = nodeptr;
      //         }
      //       }
      //     }
      //   }
      // }
      if(maxValueNode != NULL) return  maxValueNode->list.index;
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
  int idx = rand() % 11+1;
  switch(idx){
    case 1:case 11://BUILD_ROAD
      if(UNABLE_ROAD)//have not enough resource
        randAction();
      return 1;
    case 2:case 10://BUILD_NODE
      if(UNABLE_VILLAGE || UNABLE_CITY)//have not enough resource
        randAction();
      return 2;
    case 3:case 9://BUY_CARD
      if(UNABLE_BUYCARD)//have not enough resource
        randAction();
      return 3;
    case 4:case 8://USE_CARD
      forList(game->player[game->turn].devcard, element){//have not enough resource
        pDevcard cardptr = entry(element, sDevcard);
        if(cardptr->status == 1){
          return 4;
        }
      }
      randAction();
      break;
    case 5:case 7://TRADE_N
      randDiceNum();
      break;
    default:
      return 0;
      break;
  }
  return 0;
}

int randPickCard(){//random
  uint8_t member = 0;
  forList(game->player[game->turn].devcard, element){
    pDevcard cardptr = entry(element, sDevcard);
    if(cardptr->status == 1) member++;
  }
  if(member == 0) return -1;
  member = rand() % member;
  forList(game->player[game->turn].devcard, element){
    pDevcard cardptr = entry(element, sDevcard);
    if(cardptr->status == 1){
      if(member == 0){
        //cardptr->status = 2;
        return cardptr->list.index;
      }
      member--;
    }
  }
  return -1;
}

int randPickResource(){//gain resource; not random
  int minResType = 0;
  for(int i = 1 ; i < 6 ; i++){
    if(game->player[0].resource[minResType] == 0) continue;//bank has no resource
    if(game->player[game->turn].resource[minResType] > game->player[game->turn].resource[i]) continue;
    minResType = i; 
  }
  if(minResType == 0) return -1;

  return minResType;
}

int randLostResource(int32_t lostResource[5]){//robber; not random
  uint8_t totalResource = game->player[game->turn].resource[0];
  if (totalResource < 8) return -1;
  for(uint8_t i = 0 ; i < totalResource / 2 ; i++){
    int max = 1;
    for(int i = 1 ; i < 6 ; i++){
      if(game->player[game->turn].resource[max] > game->player[game->turn].resource[i]) continue;
      max = i; 
    }
    game->player[game->turn].resource[max]--;
    lostResource[max-1]++;
  }

  //add back to game->player[game->turn].resource
  for(uint8_t i = 0 ; i < 5 ; i++){
    game->player[game->turn].resource[i+1] += lostResource[i];
  }
  for(uint8_t i = 0 ; i < 6 ; i++){
    PRINTL("player %d resource %d\n", game->turn, game->player[game->turn].resource[i]);
  }
  return 0;
}

int randPickBlock(){//rodder; not random
  //find no.1 player score
  uint8_t leader = 0, leaderScore = 0;
  for(uint8_t i = 1 ; i < 4 ; i++){
    if(game->player[i].score > leaderScore && i != game->turn){
      leader = i;
      leaderScore = game->player[i].score;
    }
  }
  if(leader == 0) leader = 1;
  pBlock leaderblock = NULL;
  forList(game->block, element){
    pBlock blockptr = entry(element, sBlock);
    for(uint8_t i = 0; i < 6; i++){
      //finr most resource block
      for(uint8_t j = 0; j < 6; j++){
        if(blockptr->node[j] == NULL) continue;
        if(blockptr->node[j]->owner == game->turn) break;
        if(blockptr->node[j]->owner == leader && 
            (leaderblock == NULL || abs(blockptr->number - 7 ) < abs(leaderblock->number - 7))){
          leaderblock = blockptr;
          break;
        }
      }
    }
  }
  PRINTL("leaderblock: %ld\n", leaderblock->list.index);
  sleep(1);
  return leaderblock->list.index;
}

int randBuyCard(){
  int cnt = 0, randIdx = 0;
  forList(game->player[0].devcard, element) cnt++;
  if(cnt == 0) return -1;
  randIdx = rand() % cnt;
  pList idxList = getNode(game->player[0].devcard, randIdx);

  push(game->player[game->turn].devcard, idxList, BACK);
  //change resource to bank
  game->player[game->turn].resource[0] -= 3;
  game->player[0].resource[0] += 3;
  for(uint8_t i = 0 ; i < 3 ; i++){
    game->player[game->turn].resource[3+i]--;
    game->player[game->turn].resource[3+i]++;
  }
  return 0;
}

int randPickPlayer(int blockIdx ){
  pList pos = getNode(game->block, blockIdx);
  pBlock blockptr = entry(pos, sBlock);
  int target = 0;
  for(uint8_t i = 0 ; i < 6 ; i++){
    if(blockptr->node[i] == NULL) continue;
    if(blockptr->node[i]->owner == game->turn) continue;
    int temp = blockptr->node[i]->owner;
    if(target == 0 || (game->player[target].score < game->player[temp].score && game->player[temp].resource[0] > 0)){
      target = temp;
    }
  }
  return target;
}

int randRobPlayer(int target){
  int type = rand() % 5 + 1;
  if(game->player[target].resource[type] == 0) {
    randRobPlayer(target);
    return 0;
  }else{
    game->player[target].resource[type]--;
    game->player[game->turn].resource[type]++;
  }
  return 0;
}