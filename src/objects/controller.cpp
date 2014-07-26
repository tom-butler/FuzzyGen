#include "shared.h"
#include "controller.h"
#include "gen.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

static int random;

//util
float Lerp(float x1, float y1, float x2, float y2, float value);
void ResetAccumulator(int controller, int accumulator);
void ForceVarBounds(int controller, int var);

//init
void InitSets(int controller, int variable,short int numSets);
void InitRules(int controller, int accumulator);

//evaluate
float EvaluateRule(int controller, int accumulator, int rule); 
float EvaluateSet(int controller, int inputVar, int setID, int variable);
void EvaluateOutput(int controller, int accumulator);

void SelectHalf();
void SelectMean(float mean);

void Breed(int parents[]);
void BreedSets(int id1, int id2);
void BreedRules(int id1, int id2);

//mutate
void Mutate(int id);
void MutateCol(int controller, int var);
void MutateSet(int controller, int var, int setID);
void MutateRule(int controller, int ruleID);

float Lerp(float x1, float y1, float x2, float y2, float value) {
  //find the percentage by the x values
  float v = ((value - x1)/(x2 - x1));
  //perform lerp
  return y1 + (v * (y2 - y1));
}

void ForceVarBounds(int controller, int var){

  for(int i = 0; i < cont[controller].input[var].setNum; i++){
    Set set = cont[controller].input[var].sets[i];

    //force set formation
    if(set.leftTop > set.leftBase)
      set.leftTop = set.leftBase;
    if(set.rightTop > set.rightBase)
      set.rightTop = set.rightBase;
    if(set.leftTop < 0)
      set.leftTop = 0;
    if(set.rightTop < 0)
      set.rightTop = 0;
    if(set.leftBase < 0)
      set.leftBase = 0;
    if(set.rightBase < 0)
      set.rightBase = 0;

    //ensure they are in bounds
    if(set.centreX < cont[controller].input[var].low)
      set.centreX = cont[controller].input[var].low;
    if(set.centreX > cont[controller].input[var].high)
      set.centreX = cont[controller].input[var].high;
    if(set.centreX - set.leftTop < cont[controller].input[var].low)
      set.leftTop = set.centreX - cont[controller].input[var].low;
    if(set.centreX + set.rightTop > cont[controller].input[var].high)
      set.rightTop = cont[controller].input[var].high - set.centreX;
    if(set.centreX - set.leftBase < cont[controller].input[var].low)
      set.leftBase = set.centreX - cont[controller].input[var].low;
    if(set.centreX + set.rightBase > cont[controller].input[var].high)
      set.rightBase = cont[controller].input[var].high - set.centreX;

    //check relational compliance



    //SAVE SET
    cont[controller].input[var].sets[i] = set;
  }
}

//clean the controller accumulator variable
void ResetAccumulator(int controller, int accumulator) {
  delete[] cont[controller].output[accumulator].value;
  delete[] cont[controller].output[accumulator].scale;

  cont[controller].output[accumulator].value = new float[cont[controller].output[accumulator].ruleNum];
  cont[controller].output[accumulator].scale = new float[cont[controller].output[accumulator].ruleNum];

  cont[controller].output[accumulator].output = 0.0f;
  cont[controller].output[accumulator].active = 0;
}

//initialisation
void CreateControllers(int num_controllers, FuzzyVar input[], Accumulator output[]) {
  //seed random
  srand(static_cast <unsigned>(time(0)));

 for(int i = 0; i < num_controllers;i++) {

    //create input sets
    cont[i].input = new FuzzyVar[NUM_INPUT];
    copy(input,input+NUM_INPUT, cont[i].input);
    for(int j = 0; j < NUM_INPUT; j++)
    {
      cont[i].input[j].sets = new Set[MAX_NUM_SETS];
      if(MUT_SET_NUM){
        InitSets(i, j, GetRandInt(MIN_NUM_SETS, MAX_NUM_SETS));
      }
      else{
        InitSets(i, j,MIN_NUM_SETS);
      }
    }

    //create accumulators
    cont[i].output = new Accumulator[NUM_OUTPUT];
    copy(output,output+NUM_OUTPUT, cont[i].output);

    for(int o = 0; o < NUM_OUTPUT; o++){
      cont[i].output[o].ruleNum = 1;
      for(int p = 0; p < cont[i].output[o].varsNum; p++){
        cont[i].output[o].ruleNum *= cont[i].input[cont[i].output[o].vars[p]].setNum;
      }

      cont[i].output[o].value = new float[cont[i].output[o].ruleNum];
      cont[i].output[o].scale = new float[cont[i].output[o].ruleNum];

      //make some Rules
      cont[i].output[o].rules = new Rule[cont[i].output[o].ruleNum];
      InitRules(i, o);
    }
  }
}

//@TODO: REPLACE MULTIPLE ARRAY CALLS WITH POINTER
void InitSets(int controller, int variable,short int numSets) {
  //create initial variables
  short int start = cont[controller].input[variable].low;
  short int end = cont[controller].input[variable].high;

  short int space = (end - start)  / (numSets - 1);
  short int centre = start;
  for(int j = 0; j < numSets; j++) {

  short int lbase = (0.7 * space) + GetRandInt(0.0f, (end - start)* VARIANCE);
  short int rbase = 0.7 * space + GetRandInt(0.0f, (end - start)* VARIANCE);
  short int ltop = 0.3 * space + GetRandInt(0.0f, (end - start)* VARIANCE);
  short int rtop = 0.3 * space + GetRandInt(0.0f, (end - start)* VARIANCE);

    //check set variables for compliance
    if(centre - ltop < start) {
      ltop = centre - start;
      lbase = centre - start;
    }
    if(rtop + centre > end) {
      rtop = end - centre;
      rbase = end - centre;
    }

    //build the set
    Set s = {GetRandFloat(HEIGHT_LOW, HEIGHT_HIGH), centre, lbase, rbase, ltop, rtop};

      cont[controller].input[variable].sets[j] = s;
    //increment the centre for next set
    centre += space;
  }
  cont[controller].input[variable].setNum = numSets;
}

//initialises all rules for a given output
void InitRules(int controller, int accumulator) {
  //init the rules
  int currentRule = 0;
  for(int rule = 0; rule < cont[controller].output[accumulator].ruleNum; ++rule){
    Rule r = {0, GetRandFloat(cont[controller].output[accumulator].low,cont[controller].output[accumulator].high), 0};
    r.sets = new short int[cont[controller].output[accumulator].varsNum];
    cont[controller].output[accumulator].rules[currentRule] = r;
    ++currentRule;
  }
bool isDifferent = false;
int last = 0;
for (int i = 0; i < cont[controller].output[accumulator].varsNum; i++){
  int next = cont[controller].input[cont[controller].output[accumulator].vars[i]].setNum;
  if(i == 0)
    last = next;
  else{
    if(last != next)
      isDifferent = true;
    last = next;
  }

}


  //allocate the rules
  int set = 1;
  for(int var = 0; var < cont[controller].output[accumulator].varsNum; ++var) {
    int count = 0;
    for(int rule = 0; rule < cont[controller].output[accumulator].ruleNum; ++rule){
      if(!isDifferent){
        if(count >= cont[controller].input[var].setNum){
          set += var;
          count = 0;
        }
      }
        if(set >= cont[controller].input[var].setNum ){
          set = 0;
        }
        cont[controller].output[accumulator].rules[rule].sets[var] = set;
        ++count;
        ++set;
    }
  }
}


//evaluate all rules that have a single output
void EvaluateRules(int controller) {
  for(int o = 0; o < NUM_OUTPUT; o++){
    ResetAccumulator(controller, o);

    for(int i = 0; i < cont[controller].output[o].ruleNum; i++) {
      cont[controller].output[o].rules[i].isActive = false;
      float variable = EvaluateRule(controller,o, i);
      
      //add result
      if(variable > 0.0f) {
        cont[controller].output[o].rules[i].isActive = true;
        cont[controller].output[o].scale[cont[controller].output[o].active] = variable;
        cont[controller].output[o].value[cont[controller].output[o].active] = cont[controller].output[o].rules[i].output;
        cont[controller].output[o].active++;
      }
    }
    EvaluateOutput(controller, o);
  }
}

float EvaluateRule(int controller, int accumulator, int rule){
  cont[controller].output[accumulator].rules[rule].isActive = false;
  float bestResult = cont[controller].output[accumulator].high;
  for(int i = 0; i < cont[controller].output[accumulator].varsNum; i++){
    float result = EvaluateSet(controller, cont[controller].output[accumulator].vars[i], cont[controller].output[accumulator].rules[rule].sets[i], cont[controller].input[cont[controller].output[accumulator].vars[i]].value);
    if(result < bestResult)
      bestResult = result;
  }
  return bestResult;
}

//@TODO: repalce sets[setID] with a pointer
float EvaluateSet(int controller, int inputVar, int setID, int variable) {
  Set set = cont[controller].input[inputVar].sets[setID];
  //check if it is inside the set
  if(variable >= (set.centreX - set.leftBase) && variable <= ( set.centreX + set.rightBase))
    if(variable < set.centreX) //left
      if(variable < (set.centreX - set.leftTop))
        return Lerp(set.centreX - set.leftBase, 0.0f, set.centreX - set.leftTop, set.height, variable);
      else //variable > leftTop - centreX
        return set.height;
    else //right or centre
      if(variable > (set.centreX + set.rightTop))
        return Lerp(set.centreX + set.rightTop, set.height, set.centreX + set.rightBase, 0.0f,variable);
      else
        return set.height;
  else
    return 0;
}

void EvaluateOutput(int controller, int accumulator) {
  if(cont[controller].output[accumulator].active == 0)
    return;

  float total = 0.0f;
  float totalWeight = 0.0f;
  for(int i = 0; i < cont[controller].output[accumulator].active; i++) {
   total += cont[controller].output[accumulator].scale[i] * cont[controller].output[accumulator].value[i];
   totalWeight += cont[controller].output[accumulator].scale[i];
  }

  total /= totalWeight;
  cont[controller].output[accumulator].output = total;
}

void Select() {
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

  Breed(parents);
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
  Breed(parents);
}

void Breed(int parents[]){
  int c = 0;
  for(int i = 0; i < POP; i++) {
    //preserve best
    if(ELITISM)
      if(i == BEST_CONT)
        continue;

    if(cont[i].score != -2){

      //breed sets
      BreedSets(i, parents[c]);
      BreedRules(i,parents[c]);
      c++;

      //ParentMutation(i,parents[b]);
      float mut = GetRandFloat(0.0f, 1.0f);
      if(mut <= MUT_CHANCE)
        Mutate(i);
    }
  }
}
void BreedSets(int id1, int id2){
  random = GetRandInt(0, NUM_INPUT-1);
  delete [] cont[id1].input[random].sets;
  cont[id1].input[random].sets = new Set[MAX_NUM_SETS];
  copy(cont[id2].input[random].sets, cont[id2].input[random].sets + MAX_NUM_SETS, cont[id1].input[random].sets);
}
void BreedRules(int id1, int id2){
  /*random = GetRandInt(0, cont[id1].ruleNum -1);
  for(int i = 0; i < random; i++){
    if(cont[id1].rules[i].output != 0 && cont[id2].rules[i].output != 0){
      cont[id1].rules[i].output = cont[id2].rules[i].output;
    }
  }*/
}

void Mutate(int id) {
  random = GetRandInt(0, 2);
  if(random == 0){
    int var = GetRandInt(0,NUM_INPUT -1);
    MutateSet(id, var, GetRandInt(0,cont[id].input[var].setNum -1));
    ForceVarBounds(id, var);
  }
  else if (random == 1){
    int var = GetRandInt(0,NUM_INPUT -1);
    MutateCol(id, var);
    ForceVarBounds(id, var);
  }
/*  else{
    MutateRule(id, GetRandInt(0, cont[id].ruleNum -1 ));
  }*/

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

void MutateRule(int controller, int ruleID) {
  /*
  short int mut = GetRandInt(0,1);
  cont[controller].mutations++;
  if(cont[controller].rules[ruleID].output != 0){
    switch(mut){
      case 0: //swap the output
          random = GetRandFloat(cont[controller].output.low,cont[controller].output.high);
          cont[controller].rules[ruleID].output = random;
          break;

    }
  }
  */
}