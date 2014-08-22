#include "..\h\select.h"

#include <iostream>
#include <cstring>
#include "..\..\shared.h"
#include "..\h\breed.h"

void SelectHalf();
void SelectMean(float mean);

void SelectControllers() {
  //get some stats on this gen
  float mean = 0;
  int low = 1000;
  //get mean
  for( int i = 0; i < kNumPop; i++){
    mean += cont[i].score;
    if(low > cont[i].score)
      low = cont[i].score;
  }

  // use mean
  if(mean > 0)
    mean /= kNumPop;
  else
    mean = 0;

  MEAN_GEN = mean;
  LOW_GEN = low;
  //SelectHalf();
  SelectMean(mean);
}
void SelectHalf(){
//select highest half

  int parents[kNumAncestor];
  int c = 0;

  //get parents
  for(int i = 0; i < kNumAncestor; i++){
    int max = 0;
    int highest = 0;
    for(int j = 0; j < kNumPop; j++){
      if(cont[j].score >= max)
        highest = j;
    }
    cont[highest].score = -2;
    parents[c] = highest;
    c++;
  }

  BreedControllers(parents);
}

void SelectMean(float mean){
  int parents[kNumAncestor];
  int c = 0;
//select for breeding
  for(int i = 0; i < kNumPop; ++i) {
    if(cont[i].score >= mean){
      if(c < kNumAncestor){
        cont[i].score = -2;
        parents[c] = i;
        c++;
      }
    }
  }
//if not enough are selected
  if(c < kNumAncestor - 1){
    int unmarked[(kNumPop-1) - c];
    //int unmarked[(kNumPop-1) - c];
    memset(unmarked, 0, (kNumPop-1) - c);
    int index = 0;
    for(int i = 0; i < kNumPop; ++i){
      if(cont[i].score != -2){
        unmarked[index] = i ;
        index++;
      }
    }
    while(c < kNumAncestor){
      random = GetRandInt(0,(kNumPop-1) - c);
      cont[unmarked[random]].score = -2;
      parents[c] = unmarked[random];
      c++;
    }
    //delete [] unmarked;
  }
  BreedControllers(parents);

}
