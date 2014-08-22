#include "..\h\breed.h"

#include <algorithm>
#include <iostream>

#include "..\..\shared.h"
#include "..\h\mutate.h"
#include "..\h\create.h"

using namespace std;

void BreedVars(int id1, int id2);
void BreedSets(int id1, int id2);
void BreedRules(int id1, int id2);

void BreedControllers(int parents[]){
  int c = 0;
  for(int i = 0; i < kNumPop; i++) {
    //preserve best
    if(kElitism)
      if(i == BEST_GEN_CONTROLLER)
        continue;
    if(cont[i].score != -2){

      //breed sets
      BreedVars(i, parents[c]);
      BreedSets(i, parents[c]);
      BreedRules(i,parents[c]);
      if(i % 2 == 0)
          c++;
      //ParentMutation(i,parents[b]);
      float mut = GetRandFloat(0.0f, 1.0f);
      if(mut <= kMutationChance)
        MutateControllers(i, GetRandInt(0,kNumInput -1));
    }
  }

}
void BreedVars(int id1, int id2){
  random = GetRandInt(0, kNumInput-1);
  delete [] cont[id1].input[random].sets;
  cont[id1].input[random].sets = new Set[kNumSetsMax];

  copy(cont[id2].input[random].sets, cont[id2].input[random].sets + kNumSetsMax, cont[id1].input[random].sets);
  cont[id1].input[random].num_sets = cont[id2].input[random].num_sets;

  //regen any rules in accumulator that uses the random var
  for(int a = 0; a < kNumOutput; ++a){
    for(int v = 0; v < cont[id1].output[a].num_vars; ++v){

      if(cont[id1].output[a].vars[v] == random){
        //clean the existing rules
        for(int i = 0; i < cont[id1].output[a].num_rules; i++){
          if(cont[id1].output[a].rules[i].sets)
              delete [] cont[id1].output[a].rules[i].sets;
        }

        delete [] cont[id1].output[a].rules;
        cont[id1].output[a].num_rules = 1;
        //regen new rules
        for(int p = 0; p < cont[id1].output[a].num_vars; p++){
          cont[id1].output[a].num_rules *= cont[id1].input[cont[id1].output[a].vars[p]].num_sets;
        }
        cont[id1].output[a].rules = new  Rule[cont[id1].output[a].num_rules];
        CreateRules(id1, a);
      }
    }
  }
}
void BreedSets(int id1, int id2){
  int var  = GetRandInt(0, kNumInput-1);
  int set1 = GetRandInt(0, cont[id1].input[var].num_sets-1);
  int set2 = GetRandInt(0, cont[id2].input[var].num_sets-1);

  float old_centre = cont[id1].input[var].sets[set1].centre_x;
  cont[id1].input[var].sets[set1] = cont[id2].input[var].sets[set2];
  cont[id1].input[var].sets[set1].centre_x = old_centre;

  ForceVarBounds(id1, var);  
}
void BreedRules(int id1, int id2){
  /*random = GetRandInt(0, cont[id1].num_rules -1);
  for(int i = 0; i < random; i++){
    if(cont[id1].rules[i].output != 0 && cont[id2].rules[i].output != 0){
      cont[id1].rules[i].output = cont[id2].rules[i].output;
    }
  }*/
}