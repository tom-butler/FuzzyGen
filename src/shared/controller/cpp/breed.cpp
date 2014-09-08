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

void AsexualBreeding(int parents[]);
void BisexualBreeding(int parents[]);

void BreedControllers(int parents[]){
  if(kBreed == kAsexual)
    AsexualBreeding(parents);
  else if(kBreed == kBisexual)
    BisexualBreeding(parents);
}

void AsexualBreeding(int parents[]) {
  int c = 0;
  for(int i = 0; i < kNumPop; i++) {
    if(cont[i].score != -2){
      if(parents[c] < kNumPop){
        CleanController(cont[i]);
        CopyController(cont[parents[c]], cont[i]);
        c++;
      }
      float mut = GetRandFloat(0.0f, 1.0f);
      if(mut <= kMutationChance)
        MutateControllers(i, GetRandInt(0,kNumInput -1));
    }
  }
}

void BisexualBreeding(int parents[]) {
  int c = 0;
  for(int i = 0; i < kNumPop - 1; i += 2) {
    if(cont[i].score != -2){
      if(parents[c] < kNumPop){
        CleanController(cont[i]);
        CopyController(cont[parents[c]], cont[i]);
        CleanController(cont[i + 1]);
        CopyController(cont[parents[c + 1]], cont[i + 1]);

        //cross breed some vars
        for(int b = 0; b < (kNumInput * kBreedPercent); b++) {
        //  BreedVars(parents[c + 1], i);
        //  BreedVars(parents[c]    , i + 1);
        }
        //cross breed some sets
        for(int b = 0; b < (kNumInput * kNumSetsMax * kBreedPercent); b++) {
          BreedSets(parents[c + 1], i);
          BreedSets(parents[c]    , i + 1);
        }

        float mut = GetRandFloat(0.0f, 1.0f);
        if(mut <= kMutationChance) {
          MutateControllers(i, GetRandInt(0,kNumInput -1));
          MutateControllers(i + 1, GetRandInt(0,kNumInput -1));
        }
        c += 2;
      }
    }
  }
 for(int i = 0; i < kNumPop; i++){
    for(int var = 0; var < kNumInput; var++){
      ForceVarBounds(i, var);
    }
  }
}

//helper function for bisexual breeding
void BreedVars(int parent, int child){
  random = GetRandInt(0, kNumInput-1);
  delete [] cont[child].input[random].sets;
  cont[child].input[random] = cont[parent].input[random];
  cont[child].input[random].sets = new Set[kNumSetsMax];
  copy(cont[parent].input[random].sets, cont[parent].input[random].sets + kNumSetsMax, cont[child].input[random].sets);
  
  //regen any rules in accumulator that uses the random var
  for(int o = 0; o < kNumOutput; o++){
    for(int v = 0; v < cont[child].output[o].num_vars; v++){
      if(cont[child].output[o].vars[v] == random){

        //clean the existing rules
        for(int i = 0; i < cont[child].output[o].num_rules; i++){
          delete [] cont[child].output[o].rules[i].sets;
        }
        delete [] cont[child].output[o].scale;
        delete [] cont[child].output[o].value;
        delete [] cont[child].output[o].rules;

        //regen new rules
        cont[child].output[o].num_rules = 1;
        for(int p = 0; p < cont[child].output[o].num_vars; p++){
          cont[child].output[o].num_rules *= cont[child].input[cont[child].output[o].vars[p]].num_sets;
        }
        cont[child].output[o].value = new float[cont[child].output[o].num_rules];
        cont[child].output[o].scale = new float[cont[child].output[o].num_rules];
        cont[child].output[o].rules = new  Rule[cont[child].output[o].num_rules];
        CreateRules(child, o);
      }
    }
  }
}

//helpe function for bisxual breeding
void BreedSets(int parent, int child){
  random = GetRandInt(0, kNumInput-1);
  int child_set = GetRandInt(0, cont[child].input[random].num_sets-1);
  int parent_set = GetRandInt(0, cont[parent].input[random].num_sets-1);

  float old_centre = cont[child].input[random].sets[child_set].centre_x;
  cont[child].input[random].sets[child_set] = cont[parent].input[random].sets[parent_set];
  cont[child].input[random].sets[child_set].centre_x = old_centre;
}
