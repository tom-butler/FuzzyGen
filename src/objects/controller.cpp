#include "shared.h"
#include "controller.h"
#include "gen.h"
#include <iostream>
#include <stdlib.h>

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

//evaluate
float EvaluateSet(int controller, int inputVar, int setID, int variable);
void EvaluateOutput(int controller);

//mutate
void ParentMutation(int id1, int id2);
void ChildMutation(int id);
void MutateCollection(int id);
void MutateSet(int controller, int var, int setID);
void MutateRule(int controller, int ruleID);

//get a random integer between two low and high
short int GetRandInt(short int low, short int high){
  return rand() % (high - low) + low;
}
/*
//find the intersection of two lines
float Intersect(int x1, int y1, int x2, int y2, int input) {
  float m, b;
  //find line equation y = mx + b
  float x = x2 - x1;
  float y = y2 - y1;
  if(x == 0 || y == 0)
    m = 0;
  else
    m = y/x;
  b = y1 -m*x1;
  //get the y value of the intersection
  return m * input + b;
}
*/
float Lerp(float x1, float y1, float x2, float y2, float value) {
  //find the percentage by the x values
  value = ((value - x1)/(x2 - x1));
  //perform a lerp
  return y1 + (value * (y2 - y1));
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

//initialisation
void CreateControllers(int num_controllers, FuzzyVar input[], Accumulator output) {
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
}

void ResetVariables(int controller, FuzzyVar input[]){
  for(int i = 0; i < NUM_INPUT; i++) {
    cont[controller].input[i].value = input[i].value;
  }

  cont[controller].score = START_FUEL;
}

//@TODO: REPLACE MULTIPLE ARRAY CALLS WITH POINTER
void InitSets(int controller, int variable,short int numSets) {
  //create initial variables
  short int start = cont[controller].input[variable].low;
  short int end = cont[controller].input[variable].high;

  short int space = (end - start)  / (numSets - 1);
  short int centre = start;
  for(int j = 0; j < numSets; j++) {

  short int lbase = 0.7 * space;
  short int rbase = 0.7 * space;
  short int ltop = 0.3 * space;
  short int rtop = 0.3 * space;

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
          Rule r = {i, j,"AND", k, l, output};
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
      if(variable < set.centreX - set.leftTop)
        return Lerp(set.leftBase, 0.0f, set.leftTop, set.height, variable);
      else //variable > leftTop - centreX
        return set.height;
    else //right or centre
      if(variable > set.centreX + set.rightTop)
        return Lerp(set.rightTop, set.height, set.rightBase, 0.0f,variable);
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

void BreedControllers() {
  //select highest half and breed them
  Controller parents[ANCESTOR];
  int count = 0;
  int avg = 0;

  //find the average score
  for(int i = 0; i < POP; i++) {
    avg += cont[i].score;
  }
  if(avg != 0)
    avg /= POP;

  //get all controllers with >= avg score
  for(int i = 0; i < POP; i++) {
    if(cont[i].score >= avg) {
      //we only reset the ones that we want to keep
      ResetVariables(i, simInput);
      parents[count] = cont[i];
      count++;
      if(count >= ANCESTOR -1)
        break;
    }
  }

  //if the average didn't give enough parents, add some more randomly
  if(count < ANCESTOR){
    for(int i = count; i < ANCESTOR; i++){
      parents[i] = cont[GetRandInt(0,POP-1)];
    }
  }
  int c = 0;
  //breed the parents

  for(int i = 0; i < POP; i += 2) {
    //save the parents
    if(c < (POP -1))
      cont[++c] = parents[i];
    if(c < (POP -1))
      cont[++c] = parents[i + 1];

    //create two children from two parents
    if(c < (POP -1))
      cont[++c] = parents[i];
    if(c < (POP -1))
    cont[++c] = parents[i + 1];

    //get the id's of the children
    int id1 = c - 1;
    int id2 = c;


    //parent mutation, twice for extra variation
    ParentMutation(id1, id2);
    ParentMutation(id1, id2);

    //self mutation
    ChildMutation(id1);
    ChildMutation(id2);

    if(c >= (POP -1))
      return;
  }
}

//mutation
void ParentMutation(int id1, int id2) {

  //randomly merge two genes

  //find a random var
  random = GetRandInt(0, 4);
  short int col = GetRandInt(0, NUM_INPUT -1);
  switch(random) {
    case 0: //swap collections
    {
      FuzzyVar temp = cont[id2].input[col];
      cont[id2].input[col] = cont[id1].input[col];
      cont[id1].input[col] = temp;
      break;
    }
    case 1: //copy collection
      cont[id2].input[col] = cont[id1].input[col];
      break;
    case 2: //swap set
    {
      short int set = GetRandInt(0, NUM_SETS -1);
      Set temp = cont[id2].input[col].sets[set];
      cont[id2].input[col].sets[set] = cont[id1].input[col].sets[set];
      cont[id1].input[col].sets[set] = temp;
      break;
    }
    case 3: //copy set
    {
      short int set = GetRandInt(0, NUM_SETS -1);
      cont[id2].input[col].sets[set] = cont[id1].input[col].sets[set];
      break;
    }
    case 4: //change rule output
    {
      short int rule = GetRandInt(0,NUM_RULES -1);
      Rule temp = cont[id2].rules[rule];
      short int rule2 = GetRandInt(0,NUM_RULES -1);
      cont[id2].rules[rule].output = cont[id1].rules[rule2].output;
      break;
    }
  }

}

void ChildMutation(int id) {
  random = GetRandInt(0, 1);
  if(random == 0)
    MutateSet(id, GetRandInt(0,NUM_INPUT -1), GetRandInt(0,NUM_SETS -1));
  else
    MutateRule(id, GetRandInt(0, NUM_RULES -1));
}

//@TODO:POINTER THAT SHIZ
void MutateSet(int controller, int var, int setID) {
  short int mut = GetRandInt(0,3);
  random = GetRandInt(-VARIANCE, VARIANCE);
  cont[controller].mutations++;
  switch(mut){
    case 0: //grow top
      cont[controller].input[var].sets[setID].leftTop -= random;
      cont[controller].input[var].sets[setID].rightTop += random;
      break;
    case 1: //grow bottom
      cont[controller].input[var].sets[setID].leftBase -=random;
      cont[controller].input[var].sets[setID].rightBase +=random;
      break;
    case 2: //slide top
      cont[controller].input[var].sets[setID].leftTop +=random;
      cont[controller].input[var].sets[setID].rightTop +=random;
      break;
    case 3: //slide bottom
      cont[controller].input[var].sets[setID].leftBase +=random;
      cont[controller].input[var].sets[setID].rightBase +=random;
      break;
    default:
      return;
      break;
  }
  //ensure they are in bounds
  if(cont[controller].input[var].sets[setID].leftTop < cont[controller].input[var].low)
    cont[controller].input[var].sets[setID].leftTop = cont[controller].input[var].low;
  if(cont[controller].input[var].sets[setID].rightTop > cont[controller].input[var].low)
    cont[controller].input[var].sets[setID].rightTop = cont[controller].input[var].high;
  if(cont[controller].input[var].sets[setID].leftBase < cont[controller].input[var].low)
    cont[controller].input[var].sets[setID].leftBase = cont[controller].input[var].low;
  if(cont[controller].input[var].sets[setID].rightBase > cont[controller].input[var].high)
    cont[controller].input[var].sets[setID].rightBase = cont[controller].input[var].high;
}

void MutateRule(int controller, int ruleID) {
  short int mut = GetRandInt(0,0);
  cont[controller].mutations++;

  switch(mut){
    case 0: //swap the output
      do {
        random = GetRandInt(0, NUM_RULES -1);
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