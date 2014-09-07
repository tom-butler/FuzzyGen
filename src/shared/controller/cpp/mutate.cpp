#include "..\h\mutate.h"

#include <iostream>

#include "..\..\shared.h"


void MutateCol(int controller, int var);
void MutateSet(int controller, int var, int setID);
void MutateRules(int controller, int accumulator);
void MutateRule(int controller, int accumulator, int ruleID);

void MutateControllers(int id, int var) {
  random = GetRandInt(0, 3);
  if(random == 0){
    MutateSet(id, var, GetRandInt(0,cont[id].input[var].num_sets - 1));
  }
  else if (random == 1){
    MutateCol(id, var);
  }
  else if (random == 2){
    int accumulator = GetRandInt(0, kNumOutput - 1);
    MutateRules(id,accumulator);
  }
  else{ //random == 3
    int accumulator = GetRandInt(0, kNumOutput - 1);
    MutateRule(id,accumulator, GetRandInt(0, cont[id].output[accumulator].num_rules -1 ));
  }
}

void MutateCol(int controller, int var) {

  short int mut = GetRandInt(0, 1);
  int region = cont[controller].input[var].high - cont[controller].input[var].low;
  random = GetRandFloat(-region * 0.05f, region * 0.05f);
  switch(mut){
    case 0:
      if(kCollectionGrowMutation){
          for(int s = 0; s < cont[controller].input[var].num_sets; s++){
            cont[controller].input[var].sets[s].left_top += random;
            cont[controller].input[var].sets[s].left_base += random;
            cont[controller].input[var].sets[s].right_top += random;
            cont[controller].input[var].sets[s].right_base += random;
          }
        break;
      }
    case 1:
      if(kCollectionSlideMutation){
        for(int s = 0; s < cont[controller].input[var].num_sets; s++){
            cont[controller].input[var].sets[s].centre_x += random;
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
  cont[controller].num_mutations++;
  switch(mut){
    case 0: //grow top
      if(kSetGrowTopMutation){
        set.left_top += random;
        set.right_top += random;
        break;
      }
    case 1: //grow bottom
      if(kSetGrowBottomMutation){
        set.left_base +=random;
        set.right_base +=random;
        break;
      }
    case 2: //slide top
      if(kSetSlideTopMutation){
        set.left_top -=random;
        set.right_top +=random;
        break;
      }
    case 3: //slide bottom
      if(kSetSlideBottomMutation){
        set.left_base -=random;
        set.right_base +=random;
        break;
      }
    case 4: //shift centre
      if(kSetSlideMutation){
        set.centre_x +=random;
        break;
      }
    default:
      return;
      break;
  }
  cont[controller].input[var].sets[setID] = set;
}

void MutateRules(int controller, int accumulator) {
  cont[controller].num_mutations++;
  if(kRuleSlideMutation) {
    random = GetRandFloat(cont[controller].output[accumulator].low, cont[controller].output[accumulator].high);
    for(int r = 0; r < cont[controller].output[accumulator].num_rules; r++) {
      cont[controller].output[accumulator].rules[r].output += random;

      //if it goes over the max, loop it around
      if(cont[controller].output[accumulator].rules[r].output > cont[controller].output[accumulator].high)
        cont[controller].output[accumulator].rules[r].output -= cont[controller].output[accumulator].low;
    }
  }
}

void MutateRule(int controller, int accumulator, int ruleID) {
  cont[controller].num_mutations++;
  if(kRuleGrowMutation) {
    random = GetRandFloat(cont[controller].output[accumulator].low,cont[controller].output[accumulator].high);
    cont[controller].output[accumulator].rules[ruleID].output = random;
  }
}
