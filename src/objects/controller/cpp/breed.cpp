#include <algorithm>
#include <iostream>

#include "..\..\shared.h"
#include "..\h\mutate.h"
#include "..\h\create.h"

#include "..\h\breed.h"

using namespace std;
void BreedVars(int id1, int id2);
void BreedSets(int id1, int id2);
void BreedRules(int id1, int id2);

void BreedControllers(int parents[]){

  int c = 0;
  for(int i = 0; i < POP; i++) {
    //preserve best
    if(ELITISM)
      if(i == BEST_CONT)
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
      if(mut <= MUT_CHANCE)
        MutateControllers(i, GetRandInt(0,NUM_INPUT -1));
    }
  }

}
void BreedVars(int id1, int id2){
  random = GetRandInt(0, NUM_INPUT-1);
  delete [] cont[id1].input[random].sets;
  cont[id1].input[random].sets = new Set[MAX_NUM_SETS];

  copy(cont[id2].input[random].sets, cont[id2].input[random].sets + MAX_NUM_SETS, cont[id1].input[random].sets);
  cont[id1].input[random].setNum = cont[id2].input[random].setNum;

  //regen any rules in accumulator that uses the random var
  for(int a = 0; a < NUM_OUTPUT; ++a){
    for(int v = 0; v < cont[id1].output[a].varsNum; ++v){

      if(cont[id1].output[a].vars[v] == random){
        //clean the existing rules
        for(int i = 0; i < cont[id1].output[a].ruleNum; i++){
          if(cont[id1].output[a].rules[i].sets)
              delete [] cont[id1].output[a].rules[i].sets;
        }

        delete [] cont[id1].output[a].rules;
        cont[id1].output[a].ruleNum = 1;
        //regen new rules
        for(int p = 0; p < cont[id1].output[a].varsNum; p++){
          cont[id1].output[a].ruleNum *= cont[id1].input[cont[id1].output[a].vars[p]].setNum;
        }
        cont[id1].output[a].rules = new  Rule[cont[id1].output[a].ruleNum];
        CreateRules(id1, a);
      }
    }
  }
}
void BreedSets(int id1, int id2){
  int var  = GetRandInt(0, NUM_INPUT-1);
  int set1 = GetRandInt(0, cont[id1].input[var].setNum-1);
  int set2 = GetRandInt(0, cont[id2].input[var].setNum-1);

  float oldcentre = cont[id1].input[var].sets[set1].centreX;
  cont[id1].input[var].sets[set1] = cont[id2].input[var].sets[set2];
  cont[id1].input[var].sets[set1].centreX = oldcentre;

  ForceVarBounds(id1, var);  
}
void BreedRules(int id1, int id2){
  /*random = GetRandInt(0, cont[id1].ruleNum -1);
  for(int i = 0; i < random; i++){
    if(cont[id1].rules[i].output != 0 && cont[id2].rules[i].output != 0){
      cont[id1].rules[i].output = cont[id2].rules[i].output;
    }
  }*/
}