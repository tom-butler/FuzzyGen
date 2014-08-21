#include "..\h\mutate.h"

#include <iostream>

#include "..\..\shared.h"


void MutateCol(int controller, int var);
void MutateSet(int controller, int var, int setID);
void MutateRule(int controller, int accumulator, int ruleID);

void MutateControllers(int id, int var) {
  random = GetRandInt(0, 2);
  if(random == 0){
    MutateSet(id, var, GetRandInt(0,cont[id].input[var].setNum -1));
  }
  else if (random == 1){
    MutateCol(id, var);
  }
  else{
    int accumulator = GetRandInt(0, NUM_OUTPUT-1);
    MutateRule(id,accumulator, GetRandInt(0, cont[id].output[accumulator].ruleNum -1 ));
  }
  ForceVarBounds(id, var);
}

void MutateCol(int controller, int var) {

  short int mut = GetRandInt(0, 2);
  int region = cont[controller].input[var].high - cont[controller].input[var].low;
  random = GetRandFloat(-region * 0.05f, region * 0.05f);
  switch(mut){
    case 0:
      if(MUT_COL_GROW){
          for(int s = 0; s < cont[controller].input[var].setNum; s++){
            cont[controller].input[var].sets[s].leftTop -= random;
            cont[controller].input[var].sets[s].leftBase -= random;
            cont[controller].input[var].sets[s].rightTop += random;
            cont[controller].input[var].sets[s].rightBase += random;
          }
        break;
      }
    case 1:
      if(MUT_COL_SLIDE){
        for(int s = 0; s < cont[controller].input[var].setNum; s++){
            cont[controller].input[var].sets[s].centreX += random;
          }
        break;
      }
    case 2:
      if(MUT_COL_ADD){
          for(int s = 0; s < cont[controller].input[var].setNum; s++){
            cont[controller].input[var].sets[s].leftTop += random;
            cont[controller].input[var].sets[s].leftBase += random;
            cont[controller].input[var].sets[s].rightTop += random;
            cont[controller].input[var].sets[s].rightBase += random;
          }
        break;
      }
  }

}

void MutateSet(int controller, int var, int setID) {

  Set set = cont[controller].input[var].sets[setID];

  int region = cont[controller].input[var].high - cont[controller].input[var].low;
  random = GetRandFloat(-region * 0.05f, region * 0.05f);

  short int mut = GetRandInt(0,4);
  cont[controller].mutations++;
  switch(mut){
    case 0: //grow top
      if(MUT_SET_GROW_TOP){
        set.leftTop -= random;
        set.rightTop += random;
        break;
      }
    case 1: //grow bottom
      if(MUT_SET_GROW_BOT){
        set.leftBase -=random;
        set.rightBase +=random;
        break;
      }
    case 2: //slide top
      if(MUT_SET_SLIDE_TOP){
        set.leftTop +=random;
        set.rightTop +=random;
        break;
      }
    case 3: //slide bottom
      if(MUT_SET_SLIDE_BOT){
        set.leftBase +=random;
        set.rightBase +=random;
        break;
      }
    case 4: //shift centre
      if(MUT_SET_SLIDE){
        set.centreX +=random;
        break;
      }
    default:
      return;
      break;
  }
  cont[controller].input[var].sets[setID] = set;
}

void MutateRule(int controller, int accumulator, int ruleID) {
  cont[controller].mutations++;
  random = GetRandFloat(cont[controller].output[accumulator].low,cont[controller].output[accumulator].high);
  cont[controller].output[accumulator].rules[ruleID].output = random;
}