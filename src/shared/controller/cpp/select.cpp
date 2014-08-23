#include "..\h\select.h"

#include <iostream>
#include <cstring>
#include "..\..\shared.h"
#include "..\h\breed.h"

void SelectHalf();
void SelectAvg(float avg);

void SelectControllers() {
  //get some stats on this gen
  float avg = 0;
  int low = 1000;
  
  //get average
  for( int i = 0; i < kNumPop; i++){
    avg += cont[i].score;
    if(low > cont[i].score)
      low = cont[i].score;
  }

  // use avg
  if(avg > 0)
    avg /= kNumPop;
  else
    avg = 0;

  AVG_GEN = avg;
  LOW_GEN = low;

  if(kSelect == kSelectHalf)
    SelectHalf();
  else if(kSelect == kSelectAvg)
    SelectAvg(avg);
  else if(kSelect == kSelectAvg)
    SelectAvg(avg);
}

//selects the highest scoring half
void SelectHalf(){
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

//select
void SelectAvg(float avg){
  int parents[kNumAncestor];
  int c = 0;
//select for breeding
  for(int i = 0; i < kNumPop; ++i) {
    if(cont[i].score >= avg){
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
