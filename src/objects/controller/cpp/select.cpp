
#include "..\..\shared.h"
#include "..\h\breed.h"

#include "..\h\select.h"

void SelectHalf();
void SelectMean(float mean);

void SelectControllers() {
  //get some stats on this gen
  float mean = 0;
  int low = 1000;
  //get mean
  for( int i = 0; i < POP; i++){
    mean += cont[i].score;
    if(low > cont[i].score)
    low = cont[i].score;
  }

  // use mean
  if(mean > 0)
    mean /= POP;
  else
    mean = 0;

MEAN = mean;
LOW = low;
  //SelectHalf();
  SelectMean(mean);
}
void SelectHalf(){
//select highest half

  int parents[ANCESTOR];
  int c = 0;

  //get parents
  for(int i = 0; i < ANCESTOR; i++){
    int max = 0;
    int highest = 0;
    for(int j = 0; j < POP; j++){
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
  int parents[ANCESTOR];
  int c = 0;

//select for breeding
  for(int i = 0; i < POP; i++) {
    if(cont[i].score >= mean){
      if(c < ANCESTOR){
        cont[i].score = -2;
        parents[c] = i;
        c++;
      }
    }
  }
//if not enough are selected
  while(c < ANCESTOR){
    random = GetRandInt(0,POP-1);
    if(cont[random].score != -2){
      cont[random].score = -2;
      parents[c] = random;
      c++;
    }
  }
  BreedControllers(parents);
}
