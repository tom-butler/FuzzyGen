#include "shared.h"
#include "controller.h"
#include "gen.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

static int random;

//util
short int GetRandInt(short int low, short int high);
//float Intersect(int x1, int y1, int x2, int y2, int input);
float Lerp(float x1, float y1, float x2, float y2, float value);
//init
void ResetVariables(int controller, FuzzyVar input[]);
void InitSets(int controller, int variable,short int numSets);
void InitRules(int controller);
void InitControlController(int controller);
//evaluate
float EvaluateSet(int controller, int inputVar, int setID, int variable);
void EvaluateOutput(int controller);

void Select();
void SelectHalf();
void SelectMean();

void Breed(int parents[]);
void BreedSets(int id1, int id2);
void BreedRules(int id1, int id2);

//mutate
void Mutate(int id);
void MutateSet(int controller, int var, int setID);
void MutateRule(int controller, int ruleID);

//get a random integer between two low and high
short int GetRandInt(short int low, short int high){
  return rand() % (high - low) + low;
}

float GetRandFloat(float low, float high) {
  return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

float Lerp(float x1, float y1, float x2, float y2, float value) {
  //find the percentage by the x values
  float v = ((value - x1)/(x2 - x1));
  //perform lerp
  return y1 + (v * (y2 - y1));
}

//clean the controller accumulator variable
void ResetAccumulator(int controller) {
  delete[] cont[controller].output.value;
  delete[] cont[controller].output.scale;

  cont[controller].output.value = new float[NUM_RULES];
  cont[controller].output.scale = new float[NUM_RULES];

  cont[controller].output.output = 0.0f;
  cont[controller].output.active = 0;
}

void ResetVariables(int controller, FuzzyVar input[]){
  //for(int i = 0; i < NUM_INPUT; i++) {
 //   cont[controller].input[i].value = input[i].value;
 // }
  cont[controller].input[0].value = START_HEIGHT;
  cont[controller].input[1].value = START_VEL;

  cont[controller].score = START_FUEL;
}

//initialisation
void CreateControllers(int num_controllers, FuzzyVar input[], Accumulator output) {
  //seed random
  srand(static_cast <unsigned>(time(0)));

   for(int i = 0; i < num_controllers;i++) {

    //create input sets
    cont[i].input = new FuzzyVar[NUM_INPUT];
    copy(input,input+NUM_INPUT, cont[i].input);

    for(int j = 0; j < NUM_INPUT; j++)
    {
      cont[i].input[j].sets = new Set[NUM_SETS];
      InitSets(i, j, NUM_SETS);
    }

    //create output singletons
    cont[i].output = output;

    cont[i].output.value = new float[NUM_RULES];
    cont[i].output.scale = new float[NUM_RULES];

    //make some Rules
    cont[i].rules = new Rule[NUM_RULES];
    //give it an initial score
    cont[i].score = START_FUEL;
    InitRules(i);
  }
  if(INCLUDE_CONTROL)
    InitControlController(0);
}
//manually created controller to prove the system works
void InitControlController(int controller) {
  //height

  //landing
  cont[controller].input[0].sets[0].height = 1;
  cont[controller].input[0].sets[0].centreX = 0;
  cont[controller].input[0].sets[0].leftBase = 0;
  cont[controller].input[0].sets[0].rightBase = 50;
  cont[controller].input[0].sets[0].leftTop = 0;
  cont[controller].input[0].sets[0].rightTop = 25;

  //low
  cont[controller].input[0].sets[1].height = 1;
  cont[controller].input[0].sets[1].centreX = 60;
  cont[controller].input[0].sets[1].leftBase = 50;
  cont[controller].input[0].sets[1].rightBase = 45;
  cont[controller].input[0].sets[1].leftTop = 10;
  cont[controller].input[0].sets[1].rightTop = 10;

  //high
  cont[controller].input[0].sets[2].height = 1;
  cont[controller].input[0].sets[2].centreX = 100;
  cont[controller].input[0].sets[2].leftBase = 60;
  cont[controller].input[0].sets[2].rightBase = 900;
  cont[controller].input[0].sets[2].leftTop = 10;
  cont[controller].input[0].sets[2].rightTop = 200;

  //velocity

  //up
  cont[controller].input[1].sets[0].height = 1;
  cont[controller].input[1].sets[0].centreX = -30;
  cont[controller].input[1].sets[0].leftBase = 0;
  cont[controller].input[1].sets[0].rightBase = 28;
  cont[controller].input[1].sets[0].leftTop = 0;
  cont[controller].input[1].sets[0].rightTop = 15;

  //slow
  cont[controller].input[1].sets[1].height = 1;
  cont[controller].input[1].sets[1].centreX = 0;
  cont[controller].input[1].sets[1].leftBase = 10;
  cont[controller].input[1].sets[1].rightBase = 10;
  cont[controller].input[1].sets[1].leftTop = 5;
  cont[controller].input[1].sets[1].rightTop = 5;

  //fast
  cont[controller].input[1].sets[2].height = 1;
  cont[controller].input[1].sets[2].centreX = 30;
  cont[controller].input[1].sets[2].leftBase = 28;
  cont[controller].input[1].sets[2].rightBase = 0;
  cont[controller].input[1].sets[2].leftTop = 10;
  cont[controller].input[1].sets[2].rightTop = 0;

  //rules
  //if height landing and vel up
   Rule r0 = {0,0,"AND",1,0, 2.0f, false};
  //if height landing and vel slow
   Rule r1 = {0,0,"AND",1,1, 8.0f, false};
  //if height landing and vel fast
   Rule r2 = {0,0,"AND",1,2, 9.0f, false};

  //if height low and vel up
   Rule r3 = {0,1,"AND",1,0, 1.0f, false};
  //if height low and vel slow
   Rule r4 = {0,1,"AND",1,1, 5.0f, false};
  //if height low and vel fast
   Rule r5 = {0,1,"AND",1,2, 7.0f, false};

  //if height high and vel up
   Rule r6 = {0,2,"AND",1,0, 1.0f, false};
  //if height high and vel slow
   Rule r7 = {0,2,"AND",1,1, 5.0f, false};
  //if height high and vel fast
   Rule r8 = {0,2,"AND",1,2, 6.0f,false};

  cont[controller].rules[0] = r0;
  cont[controller].rules[1] = r1;
  cont[controller].rules[2] = r2;
  cont[controller].rules[3] = r3;
  cont[controller].rules[4] = r4;
  cont[controller].rules[5] = r5;
  cont[controller].rules[6] = r6;
  cont[controller].rules[7] = r7;
  cont[controller].rules[8] = r8;


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
/*
    cout << (centre - lbase);
    cout << " " << (centre -ltop);
    cout << " " << centre;
    cout << " " << (centre + rtop);
    cout << " " << (centre + rbase);
    cout << "\n";
*/
    //build the set
    Set s = {HEIGHT, centre, lbase, rbase, ltop, rtop};

      cont[controller].input[variable].sets[j] = s;
    //increment the centre for next set
    centre += space;
  }
}



//initialises all rules for a given output
void InitRules(int controller) {
  int currentRule = 0;
  for(int i = 0; i < NUM_INPUT; ++i) {
    for (int j = 0; j < NUM_SETS; ++j) {
      for(int k = i + 1; k < NUM_INPUT; ++k) {
        for(int l = 0; l < NUM_SETS; l++) {
          int output = GetRandInt(cont[controller].output.low,cont[controller].output.high);
          Rule r = {i, j,"AND", k, l, GetRandFloat(0.0f,THRUST_MAX), false};
          cont[controller].rules[currentRule] = r;
          ++currentRule;
        }
      }
    }
  }
}

//@TODO: THIS NEEDS A RE-WRITE (DAFUQ WAS I DOING?)
//evaluate all rules that have a single output
void EvaluateRules(int controller) {
  ResetAccumulator(controller);
  float  res1, res2 = 0.0f;
  float rcount = 0.0f;
  float returnValue = 0.0f;
  float variable;
  for(int i = 0; i < NUM_RULES; i++) {
    cont[controller].rules[i].isActive = false;
    res1 = EvaluateSet(controller, cont[controller].rules[i].inputvar, cont[controller].rules[i].inputset, cont[controller].input[cont[controller].rules[i].inputvar].value);
    res2 = EvaluateSet(controller, cont[controller].rules[i].inputvar2, cont[controller].rules[i].inputset2, cont[controller].input[cont[controller].rules[i].inputvar2].value);

    //decide on the value to pass to output
    if(cont[controller].rules[i].modifier.compare("AND") == 0) {
        if(res1 < res2)
          variable = res1;
        else
          variable = res2;
    }
    else if(cont[controller].rules[i].modifier.compare("OR") == 0) {
        if(res1 > res2)
          variable = res1;
        else
          variable = res2;
    }
    //add result
    if(variable > 0.0f) {
      cont[controller].rules[i].isActive = true;
      cont[controller].output.scale[cont[controller].output.active] = variable;
      cont[controller].output.value[cont[controller].output.active] = cont[controller].rules[i].output;
      cont[controller].output.active++;
    }
  }

   /*cout << returnValue;
   cout << " ";
   cout << rcount;
   cout << " ";
   cout << temp;
   cout << "\n";
*/
  EvaluateOutput(controller);
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

void EvaluateOutput(int controller) {
  if(cont[controller].output.active == 0)
    return;

  float total = 0.0f;
  for(int i = 0; i < cont[controller].output.active; i++) {
   total += cont[controller].output.scale[i] * cont[controller].output.value[i];
  }

  total /= cont[controller].output.active;
  cont[controller].output.output = total;
}

void Select() {
  //SelectHalf();
  SelectMean();
}
void SelectHalf(){
//select highest half

  int parents[ANCESTOR];
  int c = 0;
  int max = 0;
  int highest = 0;

  //get parents
  for(int i = 0; i < ANCESTOR; i++){
    max = 0;
    highest = 0;
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

void SelectMean(){
  int parents[ANCESTOR];
  float mean = 0;
  int c = 0;
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
    random = GetRandInt(0,POP);
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
    ResetVariables(i,simInput);
  }
}
void BreedSets(int id1, int id2){
  random = GetRandInt(0, NUM_INPUT);
  delete [] cont[id1].input[random].sets;
  cont[id1].input[random].sets =  new Set[NUM_SETS];
  copy(cont[id2].input[random].sets, cont[id2].input[random].sets + NUM_SETS, cont[id1].input[random].sets);
}
void BreedRules(int id1, int id2){
  random = GetRandInt(0, NUM_RULES);
  for(int i = 0; i < random; i++)
    cont[id1].rules[i].output = cont[id2].rules[i].output;
}

void Mutate(int id) {
  random = GetRandInt(0, 1);
  if(random == 0)
    MutateSet(id, GetRandInt(0,NUM_INPUT), GetRandInt(0,NUM_SETS));
  else
    MutateRule(id, GetRandInt(0, NUM_RULES ));
}

void MutateSet(int controller, int var, int setID) {

  Set set = cont[controller].input[var].sets[setID];

  int region = cont[controller].input[var].high - cont[controller].input[var].low;
  random = GetRandFloat(-region * 0.05f, region * 0.05f);

  short int mut = GetRandInt(0,4);
  cont[controller].mutations++;
  switch(mut){
    case 0: //grow top
      set.leftTop -= random;
      set.rightTop += random;
      break;
    case 1: //grow bottom
      set.leftBase -=random;
      set.rightBase +=random;
      break;
    case 2: //slide top
      set.leftTop +=random;
      set.rightTop +=random;
      break;
    case 3: //slide bottom
      set.leftBase +=random;
      set.rightBase +=random;
      break;
    case 4: //shift centre
      set.centreX +=random;
      break;
    default:
      return;
      break;
  }
  //ensure they actually look like sets
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

  cont[controller].input[var].sets[setID] = set;
}

void MutateRule(int controller, int ruleID) {
  short int mut = GetRandInt(0,0);
  cont[controller].mutations++;

  switch(mut){
    case 0: //swap the output
      do {
        random = GetRandInt(0, THRUST_MAX);
        if(random != cont[controller].rules[ruleID].output) {
        cont[controller].rules[ruleID].output = random;
      }
  } while(1);
      break;
    default:
      return;
      break;
  }
}