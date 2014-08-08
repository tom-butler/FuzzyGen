#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <sstream>
#include <cstring>
#include <fstream>

#include "..\sims\sim.h"

#include "shared.h"

using namespace std;
  //define the standard options ----------------

  //genetic
  short int POP = 1000;
  short int GENERATIONS = 1000;
  bool INCLUDE_CONTROL = false;
  bool LOGGING = true;
  bool RANDOM_START = false;
  bool ELITISM = false;

  bool FORCE_RELATIONAL = false;
  float FORCE_OVERLAP = 0.3f; //0 for none
  bool FORCE_COVERAGE = true;

  //mutation
  float MUT_CHANCE = 0.4f;
  float VARIANCE = 0.10f;

  bool MUT_COL_INITIAL = true;
  bool MUT_COL_GROW = true;
  bool MUT_COL_SLIDE = true;
  bool MUT_COL_ADD = true;

  bool MUT_SET_INITIAL = true;
  bool MUT_SET_NUM = true;
  bool MUT_SET_GROW_TOP = true;
  bool MUT_SET_GROW_BOT = true;
  bool MUT_SET_SLIDE = true;
  bool MUT_SET_SLIDE_TOP = true;
  bool MUT_SET_SLIDE_BOT = true;
  bool MUT_SET_ADD = true;

  bool MUT_RULE_RAND = true;
  bool MUT_RULE_ADDALL = true;

  //fuzzy
  short int MIN_NUM_SETS = 2;
  short int MAX_NUM_SETS = 4;
  float HEIGHT_LOW = 0.5;
  float HEIGHT_HIGH = 1;

  short int SIM = 2;

  //runtime variables ---------------------------
  short int NUM_INPUT = 0;
  short int NUM_OUTPUT = 0;
  bool GUI = false;
  short int ANCESTOR = POP/2;
  short int MAX_BEST = 0;
  short int BEST = 0;
  short int BEST_CONT = 0;
  float MEAN = 0.0f;
  int LOW = 0;
  int random = 0;

  string *LOG;
  FuzzyVar  *simInput;
  Accumulator * simOutput;
  FuzzyVar * simFitness;
  Controller *cont;

void InitSystem() {
  //seed random
  srand(static_cast <unsigned>(time(0)));

  SimCreateSim();
  //init system variables
  cont = new Controller[POP];
  if(LOGGING)
    LOG = new string[GENERATIONS];
}
int InitTest(int test) {
  ifstream file;
  ostringstream ss;
  ss << "tests/" << test << ".csv";
  file.open(ss.str().c_str());
  if(!file){
    file.close();
    return 1;
  }
  string line;
  while(getline(file,line)){
    string key = strtok((char *) line.c_str(),",");
    string value = strtok(NULL,",");
    if(key == "POP")
      POP = atoi(value.c_str());
    else if(key == "PARENTS")
      ANCESTOR = POP * atof(value.c_str());
    else if(key == "GENERATIONS")
      GENERATIONS = atoi(value.c_str());
    else if(key == "VARIANCE")
      VARIANCE = atof(value.c_str());
    else if(key == "MUT_CHANCE")
      MUT_CHANCE = atof(value.c_str());
    else if(key == "INCLUDE_CONTROL")
      INCLUDE_CONTROL = atoi(value.c_str());
    else if(key == "LOGGING")
      LOGGING = atoi(value.c_str());
    else if(key == "NUM_INPUT")
      NUM_INPUT = atoi(value.c_str());
    else if(key == "SIM")
      SIM = atoi(value.c_str());
  }
  file.close();
  return 0;
}

//get a random integer between two low and high
short int GetRandInt(short int low, short int high){
  return (rand() % ((high + 1) - low)) + low;
}

float GetRandFloat(float low, float high) {
  return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

float Lerp(float value, float low, float high){
  return low + (value*(high - low));
}

float Intersect(float x1, float y1, float x2, float y2, float value) {
  //find the percentage by the x values
  float v = ((value - x1)/(x2 - x1));
  //perform lerp
  return Lerp(v, y1, y2);
}

void ForceVarBounds(int controller, int var){
  Set last;
  for(int i = 0; i < cont[controller].input[var].setNum; ++i){
    Set set = cont[controller].input[var].sets[i];

    if(FORCE_COVERAGE){
      if(i == 0 && set.centreX - set.leftTop != cont[controller].input[var].low){
        set.leftTop = set.centreX - cont[controller].input[var].low;
        set.leftBase = set.leftTop;
      }
      if(i == cont[controller].input[var].setNum -1 && set.centreX + set.rightTop != cont[controller].input[var].high){
        set.rightTop = cont[controller].input[var].high -  set.centreX;
        set.rightBase = set.rightTop;
      }
    }

    //check overlap
    if(FORCE_OVERLAP > 0.0f){
      if(i == 0)
        last = set;
      else{
        //if the sets overlap less that FORCE_OVERLAP stretch them out
        if((last.centreX + last.rightBase - set.centreX - set.leftBase < (last.rightBase) * FORCE_OVERLAP) ||
           (last.centreX + last.rightBase - set.centreX - set.leftBase < (set.leftBase) * FORCE_OVERLAP)){
          //find the centre of the two sets
          float centre = last.centreX + ((set.centreX - last.centreX)/2);
          float overlap1 = (last.rightBase) * FORCE_OVERLAP;
          float overlap2 = (set.leftBase) * FORCE_OVERLAP;
          last.rightBase = centre - last.centreX + overlap1;
          set.leftBase = set.centreX - centre + overlap2;
          cont[controller].input[var].sets[i-1] = last;
        }
        last = set;
      }
    }

    //ensure they are in bounds
    if(set.centreX < cont[controller].input[var].low)
      set.centreX = cont[controller].input[var].low;
    if(set.centreX > cont[controller].input[var].high)
      set.centreX = cont[controller].input[var].high;

    if(set.centreX - set.leftTop < cont[controller].input[var].low)
      set.leftTop = set.centreX - cont[controller].input[var].low;
    if(set.centreX + set.rightTop > cont[controller].input[var].high)
      set.rightTop = cont[controller].input[var].high - set.centreX;

    //force set formation
    if(set.leftTop > set.leftBase)
      set.leftBase = set.leftTop;
    if(set.rightTop > set.rightBase)
      set.rightBase = set.rightTop;

    if(set.centreX - set.leftBase < cont[controller].input[var].low)
      set.leftBase = set.centreX - cont[controller].input[var].low;
    if(set.centreX + set.rightBase > cont[controller].input[var].high)
      set.rightBase = cont[controller].input[var].high - set.centreX;

    if(set.leftTop < 0)
      set.leftTop = 0;
    if(set.rightTop < 0)
      set.rightTop = 0;
    if(set.leftBase < 0)
      set.leftBase = 0;
    if(set.rightBase < 0)
      set.rightBase = 0;


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