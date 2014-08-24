#include "shared.h"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GetOpt.h>
#include <stdlib.h>

#include "..\sims\sim.h"

using namespace std;
  //define the standard options ----------------

  //genetic
  short int kNumPop               = 200;
  short int kNumGenerations       = 1000;
  bool kIncludeControl            = false;
  bool kLogging                   = true;
  bool kRandomStart               = true;
  short int kRandomStartTests     = 3;
  bool kElitism                   = false;

  bool kForceSetRelationship      = false;
  float kForceSetOverlap          = 0.4f; //0 for none
  bool kForceSetCoverage          = true;

  //mutation
  float kMutationChance           = 0.4f;
  float kVariance                 = 0.10f;

  bool kCollectionInitialMutaion  = true;
  bool kCollectionGrowMutation    = true;
  bool kCollectionSlideMutation   = true;
  bool kCollectionAddMutation     = true;

  bool kSetInitialMutation        = true;
  bool kSetNumberMutation         = true;
  bool kSetGrowTopMutation        = true;
  bool kSetGrowBottomMutation     = true;
  bool kSetSlideMutation          = true;
  bool kSetSlideTopMutation       = true;
  bool kSetSlideBottomMutation    = true;
  bool kSetAddMutation            = true;

  bool kRuleRandomMutation        = true;
  bool kRuleAddAllMutation        = true;

  //fuzzy
  short int kNumSetsMin           = 2;
  short int kNumSetsMax           = 3;
  float     kSetHeightMin         = 0.5;
  float     kSetHeightMax         = 1;

  short int kSim                  = kPendulumCartSim;
  short int kSelect               = kSelectAvg;

  //runtime variables ---------------------------
  short int kNumInput             = 0;
  short int kNumOutput            = 0;
  short int kNumAncestor          = kNumPop / 2;

  short int BEST_SCORE            = 0;
  short int BEST_GEN              = 0;
  short int BEST_GEN_SCORE        = 0;
  short int BEST_GEN_CONTROLLER   = 0;
  float     AVG_GEN               = 0.0f;
  int       LOW_GEN               = 0;

  int random                      = 0;

  string *LOG;
  Controller BEST_CONTROLLER;
  FuzzyVar  *simInput;
  Accumulator *simOutput;
  FuzzyVar *simFitness;
  Controller *cont;

void InitSystem() {
  //seed random
  srand(static_cast <unsigned>(time(0)));

  SimCreateSim();
  //init system variables
  cont = new Controller[kNumPop];
  if(kLogging)
    LOG = new string[kNumGenerations];
  if(!kRandomStart)
    kRandomStartTests = 0;
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
      kNumPop = atoi(value.c_str());
    else if(key == "PARENTS")
      kNumAncestor = kNumPop * atof(value.c_str());
    else if(key == "GENERATIONS")
      kNumGenerations = atoi(value.c_str());
    else if(key == "VARIANCE")
      kVariance = atof(value.c_str());
    else if(key == "MUT_CHANCE")
      kMutationChance = atof(value.c_str());
    else if(key == "INCLUDE_CONTROL")
      kIncludeControl = atoi(value.c_str());
    else if(key == "LOGGING")
      kLogging = atoi(value.c_str());
    else if(key == "NUM_INPUT")
      kNumInput = atoi(value.c_str());
    else if(key == "SIM")
      kSim = atoi(value.c_str());
  }
  file.close();
  return 0;
}

//get a random integer between two low and high
short int GetRandInt(short int low, short int high) {
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

void ForceVarBounds(int controller, int var) {
  Set last;
  for(int i = 0; i < cont[controller].input[var].num_sets; ++i){
    Set set = cont[controller].input[var].sets[i];

    if(kForceSetCoverage){
      if(i == 0 && set.centre_x - set.left_top != cont[controller].input[var].low){
        set.left_top = set.centre_x - cont[controller].input[var].low;
        set.left_base = set.left_top;
      }
      if(i == cont[controller].input[var].num_sets -1 && set.centre_x + set.right_top != cont[controller].input[var].high){
        set.right_top = cont[controller].input[var].high -  set.centre_x;
        set.right_base = set.right_top;
      }
    }

    //check overlap
    if(kForceSetOverlap > 0.0f){
      if(i == 0)
        last = set;
      else{
        //if the sets overlap less that FORCE_OVERLAP stretch them out
        if((last.centre_x + last.right_base - set.centre_x - set.left_base < (last.right_base) * kForceSetOverlap) ||
           (last.centre_x + last.right_base - set.centre_x - set.left_base < (set.left_base) * kForceSetOverlap)){
          //find the centre of the two sets
          float centre = last.centre_x + ((set.centre_x - last.centre_x)/2);
          float overlap1 = (last.right_base) * kForceSetOverlap;
          float overlap2 = (set.left_base) * kForceSetOverlap;
          last.right_base = centre - last.centre_x + overlap1;
          set.left_base = set.centre_x - centre + overlap2;
          cont[controller].input[var].sets[i-1] = last;
        }
        last = set;
      }
    }

    //ensure they are in bounds
    if(set.centre_x < cont[controller].input[var].low)
      set.centre_x = cont[controller].input[var].low;
    if(set.centre_x > cont[controller].input[var].high)
      set.centre_x = cont[controller].input[var].high;

    if(set.centre_x - set.left_top < cont[controller].input[var].low)
      set.left_top = set.centre_x - cont[controller].input[var].low;
    if(set.centre_x + set.right_top > cont[controller].input[var].high)
      set.right_top = cont[controller].input[var].high - set.centre_x;

    //force set formation
    if(set.left_top > set.left_base)
      set.left_base = set.left_top;
    if(set.right_top > set.right_base)
      set.right_base = set.right_top;

    if(set.centre_x - set.left_base < cont[controller].input[var].low)
      set.left_base = set.centre_x - cont[controller].input[var].low;
    if(set.centre_x + set.right_base > cont[controller].input[var].high)
      set.right_base = cont[controller].input[var].high - set.centre_x;

    if(set.left_top < 0)
      set.left_top = 0;
    if(set.right_top < 0)
      set.right_top = 0;
    if(set.left_base < 0)
      set.left_base = 0;
    if(set.right_base < 0)
      set.right_base = 0;


    //check relational compliance
    //SAVE SET
    cont[controller].input[var].sets[i] = set;
  }
}

//clean the controller accumulator variable
void ResetAccumulator(int controller, int accumulator) {
  delete[] cont[controller].output[accumulator].value;
  delete[] cont[controller].output[accumulator].scale;

  cont[controller].output[accumulator].value = new float[cont[controller].output[accumulator].num_rules];
  cont[controller].output[accumulator].scale = new float[cont[controller].output[accumulator].num_rules];

  cont[controller].output[accumulator].output = 0.0f;
  cont[controller].output[accumulator].num_active = 0;
}
float DegToRad(float deg) {
  return (deg * 3.14159 / 180.0);
}

float RadToDeg(float rad) {
  return (rad * 180.0 / 3.14159);
}

double sqr(double value) {
  return value*value;
}

void ForceBounds(float &var, float min, float max) {
  if(var < min)
    var = min;
  if(var > max)
    var = max;
}
void CleanController(Controller &controller) {
  for(int o = 0; o < kNumOutput; o++) {
    for(int r = 0; r < controller.output[o].num_rules; r++) {
      delete [] controller.output[o].rules[r].sets;
    }
    delete [] controller.output[o].vars;
    delete [] controller.output[o].rules;
    delete [] controller.output[o].scale;
    delete [] controller.output[o].value;
  }
  delete [] controller.output;
  for(int i = 0; i < kNumInput; i++) {
    delete [] controller.input[i].sets;
  }
  delete [] controller.input;
}
void CopyController(Controller parent, Controller &child) {
  //child = new Controller;
  child = parent;
  child.input  = new FuzzyVar[kNumInput];
  copy(parent.input , parent.input + kNumInput, child.input);
  for(int i = 0; i < kNumInput; i++) {
    child.input[i].sets = new Set[parent.input[i].num_sets];
    copy(parent.input[i].sets, parent.input[i].sets + parent.input[i].num_sets, child.input[i].sets);
  }
  child.output = new Accumulator[kNumOutput];
  copy(parent.output, parent.output + kNumOutput, child.output);
  for(int o = 0; o < kNumOutput; o++){
    child.output[o].value = new float[parent.output[o].num_rules];
    copy(parent.output[o].value, parent.output[o].value + parent.output[o].num_rules, child.output[o].value);
    
    child.output[o].scale = new float[parent.output[o].num_rules];
    copy(parent.output[o].scale, parent.output[o].scale + parent.output[o].num_rules, child.output[o].scale);
    
    child.output[o].rules = new Rule[parent.output[o].num_rules];
    copy(parent.output[o].rules, parent.output[o].rules + parent.output[o].num_rules, child.output[o].rules);
    
    child.output[o].vars = new short int[parent.output[o].num_vars];
    copy(parent.output[o].vars , parent.output[o].vars + parent.output[o].num_vars , child.output[o].vars);
    for(int r = 0; r < parent.output[o].num_rules; r++){
      child.output[o].rules[r].sets = new short int[parent.output[o].num_vars];
      copy(parent.output[o].rules[r].sets , parent.output[o].rules[r].sets + parent.output[o].num_vars , child.output[o].rules[r].sets);
    
    }
  }

}